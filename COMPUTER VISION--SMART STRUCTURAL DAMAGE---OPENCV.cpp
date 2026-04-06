/*
 * Structural Damage Detection using OpenCV
 * Detects cracks, rust/corrosion, and thermal damage from images
 * Author: Kelvin Mwirigi 
 */

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

// ----- Function declarations -----
void detectCracks(const Mat& img);
void detectRust(const Mat& img);
void detectThermalDamage(const Mat& img);

int main() {
    // ---- Load an image ----
    string imagePath;
    cout << "Enter path to image (e.g., test.jpg): ";
    cin >> imagePath;

    Mat image = imread(imagePath);
    if (image.empty()) {
        cerr << "Error: Cannot load image!" << endl;
        return -1;
    }

    resize(image, image, Size(), 0.6, 0.6); // Resize for easier viewing
    imshow("Original Image", image);

    // ---- Run detections ----
    detectCracks(image);
    detectRust(image);
    detectThermalDamage(image);

    waitKey(0);
    return 0;
}

// ====================================================
// ==============  CRACK DETECTION ====================
// ====================================================
void detectCracks(const Mat& img) {
    Mat gray, blurred, edges;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, blurred, Size(5, 5), 0);

    // Edge detection (detects fine cracks)
    Canny(blurred, edges, 60, 120);

    // Morphological filtering to refine cracks
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    dilate(edges, edges, kernel);
    erode(edges, edges, kernel);

    // Find contours (potential cracks)
    vector<vector<Point>> contours;
    findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    Mat crackMask = Mat::zeros(img.size(), CV_8UC3);
    int crackCount = 0;

    for (size_t i = 0; i < contours.size(); i++) {
        double length = arcLength(contours[i], false);
        Rect box = boundingRect(contours[i]);

        // Filter small contours (noise)
        if (length > 100 && box.width < 30) {
            drawContours(crackMask, contours, (int)i, Scalar(0, 0, 255), 2);
            crackCount++;
        }
    }

    Mat output;
    addWeighted(img, 0.7, crackMask, 1.0, 0, output);
    putText(output, "Cracks detected: " + to_string(crackCount),
            Point(20, 30), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 0, 255), 2);

    imshow("Crack Detection", output);
    cout << "[INFO] Cracks detected: " << crackCount << endl;
}

// ====================================================
// ==============  RUST DETECTION =====================
// ====================================================
void detectRust(const Mat& img) {
    Mat hsv, mask, rustMask;
    cvtColor(img, hsv, COLOR_BGR2HSV);

    // Define HSV range for rust (reddish-brown)
    Scalar lowerRust(5, 100, 20);  // Lower bound of brownish-red
    Scalar upperRust(25, 255, 255);

    inRange(hsv, lowerRust, upperRust, mask);

    // Smooth and clean mask
    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
    morphologyEx(mask, mask, MORPH_OPEN, kernel);
    morphologyEx(mask, mask, MORPH_CLOSE, kernel);

    // Calculate rust area
    double rustPixels = countNonZero(mask);
    double totalPixels = img.rows * img.cols;
    double rustPercent = (rustPixels / totalPixels) * 100.0;

    // Highlight rust area
    Mat rustHighlight = img.clone();
    rustHighlight.setTo(Scalar(0, 0, 255), mask);

    Mat output;
    addWeighted(img, 0.7, rustHighlight, 0.6, 0, output);

    putText(output, "Rust Area: " + to_string((int)rustPercent) + "%",
            Point(20, 30), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 0, 255), 2);

    imshow("Rust Detection", output);
    cout << "[INFO] Rust area: " << rustPercent << " %" << endl;
}

// ====================================================
// ===========  THERMAL DAMAGE DETECTION ==============
// ====================================================
void detectThermalDamage(const Mat& img) {
    Mat gray, heatMask;
    cvtColor(img, gray, COLOR_BGR2GRAY);

    // Threshold high brightness (hot areas)
    threshold(gray, heatMask, 200, 255, THRESH_BINARY);

    // Count hot area
    double hotPixels = countNonZero(heatMask);
    double totalPixels = img.rows * img.cols;
    double heatPercent = (hotPixels / totalPixels) * 100.0;

    Mat heatColor;
    applyColorMap(heatMask, heatColor, COLORMAP_JET);

    Mat output;
    addWeighted(img, 0.7, heatColor, 0.8, 0, output);
    putText(output, "Thermal damage: " + to_string((int)heatPercent) + "%",
            Point(20, 30), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255, 255, 255), 2);

    imshow("Thermal Damage Detection", output);
    cout << "[INFO] Thermal damage area: " << heatPercent << " %" << endl;
}
