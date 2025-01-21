//Згенерувати елыпс та задати точку. Якщо точка в еліпсі задати вектор руху. Змоделювати рух точкі в еліпсі
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

using namespace std;
using namespace sf;

bool pointInElipse(Vector2f center, Vector2f pointPos, float a, float b);
float lenVectors (Vector2f coordinateVector);

int main()
{
    Vector2f ellipseCenter, vectorMove, pointPosition, pointPosition2, dott;
    float semiMajorAxis, semiMinorAxis;
    bool minusAngle = false;
    bool pointMove = false;

    cout << "Enter the center coordinates of the ellipse (x y): ";
    cin >> ellipseCenter.x >> ellipseCenter.y;
    cout << "Enter the semi-major axis: ";
    cin >> semiMajorAxis;
    cout << "Enter the semi-minor axis: ";
    cin >> semiMinorAxis;

    RenderWindow window(VideoMode(1000, 1000), "Ellipse");

    CircleShape ellipse(semiMajorAxis);
    ellipse.setPosition(ellipseCenter);
    ellipse.setOrigin(semiMajorAxis, semiMajorAxis);
    ellipse.setScale(1, semiMinorAxis/semiMajorAxis);
    ellipse.setFillColor(Color::Blue);


    CircleShape point(5);
    point.setOrigin(5, 5);
    point.setFillColor(Color::Red);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && !pointMove){
                pointPosition = window.mapPixelToCoords(Mouse::getPosition(window));
                pointPosition2 = pointPosition;
                point.setPosition(pointPosition);
                window.clear(Color::White);
                window.draw(ellipse);
                window.draw(point);
                window.display();

                cout << "First X: " << pointPosition2.x << " First Y: " << pointPosition2.y << "\n";
                if (pointInElipse(ellipseCenter, pointPosition, semiMajorAxis, semiMinorAxis)) {
                    cout << "Enter the motion vector (x, y): ";
                    cin >> vectorMove.x >> vectorMove.y;
                    vectorMove /= lenVectors(vectorMove);
                    pointMove = true;
                }
                else{
                    cout << "Point doesn`t inside ellipse\n";
                }
            }
        }

        bool pointInside = pointInElipse(ellipseCenter, pointPosition, semiMajorAxis, semiMinorAxis);

        if(pointMove && pointInside){
            pointPosition += vectorMove/ 10.f ;
            point.setPosition(pointPosition);
            dott = Vector2f(pointPosition.x, pointPosition.y);
        }

        else if (pointMove && !pointInside){
            cout << "Dott x: " << dott.x << " Dott.y: " << dott.y << "\n";
            //Малювання прямої з точки відправлення до дотичної
            VertexArray line (Lines, 2);
            line[0].position = pointPosition2;
            line[1].position = dott;
            line[0].color = Color::Yellow;
            line[1].color = Color::Yellow;

            Vector2f dottVectorCord = Vector2f ((semiMinorAxis * semiMinorAxis * (dott.x - ellipseCenter.x)),
                     (semiMajorAxis * semiMajorAxis * (dott.y - ellipseCenter.y)));
            float lenDottVector = lenVectors(dottVectorCord);
            dottVectorCord = Vector2f (-dottVectorCord.y/lenDottVector, dottVectorCord.x/lenDottVector);
            cout << "Normal vector to dott: " << dottVectorCord.x << " " << dottVectorCord.y << "\n";

            //Намалюємо дотичну до еліпса
            VertexArray dottLine (Lines, 2);
            dottLine[0].position = dott - dottVectorCord * 200.f;
            dottLine[1].position = dott + dottVectorCord * 200.f;
            dottLine[0].color = Color::White;
            dottLine[1].color = Color::White;

            //Знвйдемо перпендикуляр
            Vector2f perpendicularToDott = Vector2f(-dottVectorCord.y, dottVectorCord.x);

            //Умова, щоб перпендикуляр був напрямлеий всередину
            Vector2f pointPependicularCheck = dott - 50.f * perpendicularToDott;
            if (!pointInElipse(ellipseCenter, pointPependicularCheck, semiMajorAxis, semiMinorAxis)){
                    cout << "Changed";
                    pointPependicularCheck = dott + 50.f * perpendicularToDott;}

            //Намалюємо пряму вектора нормального
            VertexArray normalLine (Lines, 2);
            normalLine[0].position = pointPependicularCheck;
            normalLine[1].position = dott;
            dottLine[0].color = Color::Green;
            dottLine[1].color = Color::Green;

            //Знайдемо вектор і довжину прямої з початкової точки до дотичної
            Vector2f vectorStartLine = dott - pointPosition2;
            cout << "VectorStartLine: " << vectorStartLine.x << " " << vectorStartLine.y << "\n";
            float lenStartLine = lenVectors(vectorStartLine);
            cout << "Lenght start line: " << lenStartLine << "\n";
            vectorStartLine /= lenStartLine;
            cout << "VectorStartLine(Normilized): " << vectorStartLine.x << " " << vectorStartLine.y << "\n";

            //Нормалізуємо перпендикулярний до дотичної вектор
            Vector2f perpendicularVectorCord = dott - pointPependicularCheck;
            float lenPerpendicular = lenVectors(perpendicularVectorCord);
            perpendicularVectorCord /= lenPerpendicular;
            cout << "Perpendicular: " << perpendicularVectorCord.x << " " << perpendicularVectorCord.y << "\n";

            //Оскільки вектори вже нормалізовані, то добуток їх довжин дорівнює 1
            //Знайдемо кут між ними
            float cosAngle = vectorStartLine.x * perpendicularVectorCord.x + vectorStartLine.y * perpendicularVectorCord.y;
            cout << "Cos angle: " << cosAngle << "\n";
            int angleDegree = acos(cosAngle)* (180.0f/3.14159f);
            cout << "Cos degree: " << angleDegree << "\n";

            //Перевірка чи лежить вектор ліворуч чи праворуч перпендикуляра
            float vectorProduct = perpendicularVectorCord.x*vectorStartLine.y - perpendicularVectorCord.y * vectorStartLine.x;

            //Повернемо точку
            if (vectorProduct > 0){angleDegree *= -2;}
            else angleDegree *= 2;
            float rotatedX = (pointPosition2.x - dott.x) * cos(angleDegree  * M_PI/180) - (pointPosition2.y - dott.y) * sin(angleDegree * M_PI / 180) + dott.x;
            float rotatedY = (pointPosition2.x - dott.x) * sin(angleDegree  * M_PI / 180) + (pointPosition2.y - dott.y) * cos(angleDegree * M_PI / 180) + dott.y;
            pointPosition2 = Vector2f(rotatedX, rotatedY);

            VertexArray reflectLine (Lines, 2);
            reflectLine[0].position = dott;
            reflectLine[1].position = pointPosition2;
            reflectLine[0].color = Color::Magenta;
            reflectLine[1].color = Color::Magenta;

            //Знайдемо новий вектор руху
            Vector2f newVectorCord = Vector2f (pointPosition2.x - dott.x, pointPosition2.y - dott.y);
            float lenNewVector = lenVectors(newVectorCord);
            newVectorCord /= lenNewVector;
            cout << "New vector: " << newVectorCord.x << " " << newVectorCord.y << "\n";

            window.draw(line);
            window.draw(dottLine);
            window.draw(normalLine);
            window.draw(reflectLine);
            window.display();

            int a;
            cin >> a;

            vectorMove = Vector2f(newVectorCord.x, newVectorCord.y);
            pointPosition2 = Vector2f(dott.x, dott.y);

            cout << "Vector move: " << vectorMove.x << " " << vectorMove.y << "\n";

            pointPosition = dott;
            pointPosition += vectorMove;
            point.setPosition(pointPosition);

        }
        window.clear(Color::White);
        window.draw(ellipse);
        window.draw(point);
        window.display();
    }
    return 0;
}


bool pointInElipse(Vector2f center, Vector2f pointPos, float a, float b){
    float res1 = ((center.x - pointPos.x) / a);
    float res2 = ((center.y - pointPos.y) / b);
    return res1 * res1 + res2 * res2 < 1;
}

float lenVectors (Vector2f coordinateVector){
    return sqrt(coordinateVector.x * coordinateVector.x + coordinateVector.y * coordinateVector.y);
}
