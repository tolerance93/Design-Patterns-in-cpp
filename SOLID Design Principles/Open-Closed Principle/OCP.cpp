#include <iostream>
#include <vector>
#include <boost/lexical_cast.hpp>
using namespace std;

enum class Color { red, green, blue};
enum class Size { small, medium, large};

struct Product
{
    string name;
    Color color;
    Size size;
};

struct ProductFilter
{
    typedef vector<Product*> Items;
    
    Items by_color(Items items, const Color color)
    {
        Items result;
        //in order to avoid creating a copy of the elements inside the vector
        for (auto& i: items)
            if (i->color == color)
                result.push_back(i);
        return result;
    }
    
    //클라이언트에게 ProductFilter를 보낸 후에 조건이 추가된다면 다시 컴파일해야한다.. OCP를 어김. + 코드 중복
    Items by_size(Items items, const Size size)
    {
        Items result;
        for (auto& i : items)
            if (i->size == size)
                result.push_back(i);
        return result;
    }
    
    Items by_size_and_color(Items items, const Size size, const Color color)
    {
        Items result;
        for (auto& i : items)
            if (i->size == size && i->color == color)
                result.push_back(i);
        return result;
    }
};

//forward declaration
template <typename T> struct AndSpecification;

template <typename T> struct Specification
{
    virtual ~Specification() = default;
    /*
            For a virtual function you need to provide implementation in the base class. However derived class can override this implementation with its own implementation. Normally , for pure virtual functions implementation is not provided. You can make a function pure virtual with =0 at the end of function declaration. Also, a class containing a pure virtual function is abstract i.e. you can not create a object of this class.
        */
    virtual bool is_satisfied(T* item) const = 0;
};

template <typename T> AndSpecification<T> operator&& (const Specification<T>& first, const Specification<T>& second)
{
    return {first, second};
}

template <typename T> struct Filter
{
    virtual vector<T*> filter(vector<T*> items, Specification<T>& spec) = 0;
};

struct BetterFilter : Filter<Product>
{
    vector<Product*> filter(vector<Product*> items,
                            Specification<Product> &spec) override
    {
        vector<Product*> result;
        for (auto& p: items)
            if (spec.is_satisfied(p))
                result.push_back(p);
        return result;
    }
};

struct ColorSpecification : Specification<Product>
{
    Color color;
    
    ColorSpecification(Color color) : color(color) {}
    bool is_satisfied(Product *item) const override
    {
        return item->color == color;
    }
};

struct SizeSpecification : Specification<Product>
{
    Size size;
    
    explicit SizeSpecification(const Size size) : size{ size }
    {
    }
    
    bool is_satisfied(Product* item) const override
    {
        return item->size == size;
    }
};

template <typename T> struct AndSpecification : Specification<T>
{
    const Specification<T>& first;
    const Specification<T>& second;
    
    AndSpecification(const Specification<T>& first, const Specification<T>& second) : first(first), second(second) {}
    
    bool is_satisfied(T *item) const override
    {
        return first.is_satisfied(item) && second.is_satisfied(item);
    }
};

int main()
{
  Product apple{"Apple", Color::green, Size::small};
  Product tree{"Tree", Color::green, Size::large};
  Product house{"House", Color::blue, Size::large};

  const vector<Product*> all { &apple, &tree, &house };

  BetterFilter bf;
  ColorSpecification green(Color::green);
  auto green_things = bf.filter(all, green);
  for (auto& x : green_things)
    cout << x->name << " is green\n";


  SizeSpecification large(Size::large);
  AndSpecification<Product> green_and_large(green, large);

  //auto big_green_things = bf.filter(all, green_and_large);

  // use the operator instead (same for || etc.)
  auto spec = green && large;
  for (auto& x : bf.filter(all, spec))
    cout << x->name << " is green and large\n";

  // warning: the following will compile but will NOT work
  // auto spec2 = SizeSpecification{Size::large} &&
  //              ColorSpecification{Color::blue};

  getchar();
  return 0;
}
