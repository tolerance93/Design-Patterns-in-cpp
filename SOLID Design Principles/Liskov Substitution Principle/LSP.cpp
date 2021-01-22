/**
 * Objects in a program should be replaceable with instances of their subtypes
 * without altering the correctness of the program
 * 자식 객체를 부모와 동등하게 취급할 수 있어야 한다.
 */

#include <iostream>

class Rectangle
{
protected:
    int width, height;
public:
    Rectangle(const int width, const int height) : width{width}, height{height} {};
    //this로 가리킬 수 있는 것들을 바꿀 수 없음
    int get_width() const { return width; }
    virtual void set_width(const int width) { this->width = width; }
    int get_height() const { return height; }
    virtual void set_height(const int height) { this->height = height; }
    bool is_square() const;
    
    int area() const { return width * height; }
};

class Square : public Rectangle
{
public:
    Square(int size) : Rectangle(size,size) {}
    void set_width(const int width) override {
        this->width = height = width;
    }
    void set_height(const int height) override {
        this->height = width = height;
    }
};


void process(Rectangle& r)
{
    int w = r.get_width();
    r.set_height(10);
    
    std::cout << "expected area = " << (w * 10)
    << ", got :" << r.area() << std::endl;
}

/*
 * 애당초 서브 클래스를 만들지 않기. 팩토리를 두어 따로따로 생성
 */
struct RectangleFactory
{
    static Rectangle create_rectangle(int w, int h);
    static Rectangle create_square(int size);
};

/*
 * 정사각형의 여부를 확인해야 할 수도 있다. ex)process()
 */
bool Rectangle::is_square() const
{
    return width == height;
}
