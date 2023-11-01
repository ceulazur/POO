#include <fn.hpp>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>

class Point2D {
public:
    double x;
    double y;

    Point2D(double x, double y): x(x), y(y) {}
    
    std::string str() const { 
        return fn::format("({%.2f}, {%.2f})", x, y);
    }
};

std::ostream& operator<<(std::ostream& os, const Point2D& p) { return os << p.str(); }

class Math {
public:
//método estático para calcular a distância entre dois pontos
    static double distance(Point2D p1, Point2D p2) { 
        return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
    }
};

class Shape {
    std::string name;
public:
    Shape(std::string name = "") {}

    virtual ~Shape(){}

    virtual bool inside(Point2D p) const = 0;

    virtual double getArea() const = 0;

    virtual double getPerimeter() const = 0;

    virtual std::string getName() const { return {}; }

    virtual std::string str() const { return {}; }

};

std::ostream& operator<<(std::ostream& os, const Shape& p) { return os << p.str(); }

class Circle : public Shape {
    Point2D center;
    double radius;
public:
    Circle(Point2D center, double radius): center(center), radius(radius) { }

    bool inside(Point2D p) const override { 
        return Math::distance(center, p) <= radius;
    }
    
    //retorna a área da forma
    //implementa os métodos abstratos
    double getArea() const override { 
        return M_PI * radius * radius;
    }

    double getPerimeter() const override {
        return 2 * M_PI * radius;
    }
   //retorna o nome da classe e os valores no seguinte formato
    //Circ: C=(0.00, 0.00), R=0.00
    //O ponto deve ser gerado pelo método toString da classe Point2D
    std::string str() const override {
        return fn::format("Circ: C={}, R={%.2f}", center.str(), radius);
    }

    std::string getName() const override { 
        return "Circ";
    }
};

class Rectangle : public Shape {
    Point2D p1;
    Point2D p2;

public:
    Rectangle(Point2D p1, Point2D p2): p1(p1), p2(p2) {};

    bool inside(Point2D p) const override { 
        return p1.x <= p.x && p.x <= p2.x && p1.y <= p.y && p.y <= p2.y;
    }

    double getArea() const override { 
        return std::abs(p1.x - p2.x) * std::abs(p1.y - p2.y);
    }

    double getPerimeter() const override { 
        return 2 * (std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y));
    }

    Point2D getP1() const { 
        return p1;
    }

    Point2D getP2() const { 
        return p2; 
    }

    std::string str() const override { 
        return fn::format("Rect: P1={} P2={}", p1.str(), p2.str());
    }

    std::string getName() const override { 
        return "Rect";
    }
};


int main() {
    std::vector<std::shared_ptr<Shape>> shapes;

    while (true) {
        auto line = fn::input();
        auto args =  fn::split(line, ' ');
        fn::write("$" + line);

        if (args[0] == "end")  { 
            break;                                  
        }
        else if (args[0] == "show") { 
            for (auto shape : shapes) {
                fn::write(shape->str());
            }
        }
        else if (args[0] == "circle") { 
            auto x = +args[1];
            auto y = +args[2]; 
            auto r = +args[3];
            shapes.push_back(std::make_shared<Circle>(Point2D{x, y}, r));
        }
        else if (args[0] == "rect") { 
            auto p1 = Point2D{+args[1], +args[2]};
            auto p2 = Point2D{+args[3], +args[4]};
            shapes.push_back(std::make_shared<Rectangle>(p1, p2));
        }
        else if (args[0] == "info") {
            for (auto shape : shapes) {
                fn::print("{}: A={%.2f} P={%.2f}\n", 
                shape->getName(), shape->getArea(), shape->getPerimeter());
            }
        }
        else {
            fn::write("fail: invalid command");
        }
    }
}
