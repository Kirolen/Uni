//Намалювати трикутник. За бажанням повернути його на n градусів та збільшити у k разів
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;
using namespace sf;

int main() {
    RenderWindow window(VideoMode(800, 800), "Triangle");

    vector<Vector2f> trianglePos;
    VertexArray triangle(Triangles, 3);

    CircleShape centerPoint(5);
    Vector2f center;

    bool changeTriangle = false;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event)){
            if (event.type == Event::Closed){
                window.close();
            }

            if (changeTriangle){

                changeTriangle = false;
                string answer;

                do{
                cout << "Do you want to rotate the triangle?(y/n): ";
                cin >> answer;
                }
                while (answer != 'Y' && answer != 'y'&& answer != 'N' && answer != 'n' );

                cout << "Center: " << center.x << " " << center.y << "\n";

                if (answer == 'Y' || answer == 'y'){
                    window.clear();

                    int angle;
                    cout << "Enter the number of degrees: ";
                    cin >> angle;

                    cout << "New triangle positions after rotate: \n";

                    for (int i = 0; i < 3; ++i) {
                        float rotatedX = (triangle[i].position.x - center.x) * cos(angle * 3.14159265 / 180) - (triangle[i].position.y - center.y) * sin(angle * 3.14159265 / 180) + center.x;
                        float rotatedY = (triangle[i].position.x - center.x) * sin(angle * 3.14159265 / 180) + (triangle[i].position.y - center.y) * cos(angle * 3.14159265 / 180) + center.y;
                        triangle[i].position = Vector2f(rotatedX, rotatedY);
                        cout << "x" << i + 1 << ": " << static_cast<int>(triangle[i].position.x) << " y" << i + 1 << ": " << static_cast<int>(triangle[i].position.y) << "\n";
                    }
                    window.clear();
                    window.draw(triangle);
                    window.draw(centerPoint);
                    window.display();
                }


                do {
                cout << "Do you want to change the size of the triangle?(y/n): ";
                cin >> answer;
                }
                while  (answer != 'Y' && answer != 'y'&& answer != 'N' && answer != 'n' );


                if (answer == 'Y' || answer == 'y'){
                    float changeSize;
                    do{
                    cout << "Enter a number by how much you want to change the triangle: ";
                    cin >> changeSize;
                    }
                    while(changeSize < 0);

                    cout << "New triangle positions after scaling: \n";
                    for (int j = 0; j < 3; j++){
                        triangle[j].position = center + changeSize * (triangle[j].position - center);
                        cout << "x" << j + 1 << ": " << static_cast<int>(triangle[j].position.x) << " y" << j + 1 << ": " << static_cast<int>(triangle[j].position.y) << "\n";
                    }
                    window.clear();
                    window.draw(triangle);
                    window.draw(centerPoint);
                    window.display();
                }
                trianglePos.clear();
            }

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left){
                Vector2f mousePosition = window.mapPixelToCoords(Mouse::getPosition(window));
                trianglePos.push_back(mousePosition);
            }
            if (trianglePos.size() == 3 && !changeTriangle){
                cout << "Triangle coordinates: \n";
                for (int i = 0; i < 3; i++) {
                    triangle[i].position = trianglePos[i];
                    triangle[i].color = Color::Red;
                }
                changeTriangle = true;
            }

        window.clear();
        window.draw(triangle);
        window.draw(centerPoint);
        window.display();
        }
    }
}
