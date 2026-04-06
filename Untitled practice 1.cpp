#include <iostream>
using namespace std;
class vec2 {
    public:
    float x,y;
    vec2 (float x ,float y) :x(x) , y(y) {}
    vec2 operator +(const vec2& rhs) const {
        return vec2(x+rhs.x ,y+rhs.y);
    }


    };
int main() {
    vec2 v1(1,2);
    vec2 v2(4,5);
    vec2 v3(7,4);
    vec2 result = v1+v2+v3;
    cout << "result:" << result.x << "," << result.y << "\n";
    return 0;
}

