#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
using namespace std;

/**
 * 빌더 패턴은 생성이 까다로운 객체를 쉽게 처리하기 위한 패턴. 즉 생성자 코드 단 한 줄로 생성할 수 없는 객체를 다룸
 */

struct HtmlBuilder;

struct HtmlElement
{
    string name;
    string text;
    vector<HtmlElement> elements;
    const size_t indent_size = 2;
    
    HtmlElement() {}
    HtmlElement(const string& name, const string& text) : name(name), text(text)
    {
    }
    
    string str(int indent = 0) const
    {
        ostringstream oss;
        string i(indent_size*indent, ' ');
        oss << i << "<" << name << ">" << endl;
        if (text.size() > 0)
            oss << string(indent_size*(indent + 1), ' ') << text << endl;
        
        for (const auto& e : elements)
            oss << e.str(indent + 1);
        
        oss << i << "</" << name << ">" << endl;
        return oss.str();
    }
    
    /**
     * 팩토리 메서드를 두어 빌더를 생성할 수 있게한다.
     * 생성자를 private/protected로 감춰서 build함수가 아니면 객체 생성이 불가능하도록 강제할 수 있다.
     * static method!
     */
    
    static unique_ptr<HtmlBuilder> build(string root_name)
    {
        return make_unique<HtmlBuilder>(root_name);
    }
};

struct HtmlBuilder
{
    HtmlBuilder(string root_name)
    {
        root.name = root_name;
    }
    
    /**
     * 리턴 값을 활용하면 흐림식 인터페이스 스타일의 빌더를 만들 수 있다.(fluent interface)
     * void to start with
     */
    
    HtmlBuilder& add_child(string child_name, string child_text)
    {
        HtmlElement e{ child_name, child_text };
        
        /**
         * push_back vs emplace_back
         * Instead of taking a value_type it takes a variadic list of arguments,
         * so that means that you can now perfectly forward the arguments and
         * construct directly an object into a container without a temporary at all.
         */
        root.elements.emplace_back(e);
        return *this;
    }
    
    //pointer based
    HtmlBuilder* add_child_2(string child_name, string child_text)
    {
        HtmlElement e{ child_name, child_text };
        root.elements.emplace_back(e);
        return this;
    }
    
    string str() { return root.str(); }
    
    /**
     * 최종 목적은 HtmlElement를 만드는 것이지, 빌더를 만드는 것이 아니다.
     * 단순 root return대신 return std::move(root)와 같이 이동 시멘틱을 이용할 수도 있다.
     * conversion operator. 이후에는 copy constructor를 통해 할당( ex. = 사용시)
     */
    
    operator HtmlElement() const { return root; }
    HtmlElement root;
};

int main()
{
    // <p>hello</p>
    auto text = "hello";
    string output;
    output += "<p>";
    output += text;
    output += "</p>";
    printf("<p>%s</p>", text);
    
    // <ul><li>hello</li><li>world</li></ul>
    string words[] = { "hello", "world" };
    ostringstream oss;
    oss << "<ul>";
    for (auto w : words)
      oss << "  <li>" << w << "</li>";
    oss << "</ul>";
    printf(oss.str().c_str());
    cout << endl;
    //easier
    HtmlBuilder builder{ "ul" };
    builder.add_child("li", "hello").add_child("li", "world");
    cout << builder.str() << endl;
    
    /**
     * conversion operator  + copy constructor 사용
     * xCode에서는 한줄에 fluent로 불가. why?
     */
    auto builder2 = HtmlElement::build("ul");
    builder2->add_child_2("li", "hello")->add_child_2("li", "world");
    cout << builder2->str() << endl;
    
    getchar();
    return 0;
}
