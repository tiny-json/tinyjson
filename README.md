# tinyjson
A small lightweight Json c++ library that features:
 - Header only
 - Simple syntax
 - Full unicode support
 - Support full JSON data type, parsing, serialization

# Sample usage:

    json w = parser::parse(R"(
        {"widget": {
            "debug": "on",
            "window": {
                "title": "Sample Konfabulator Widget",
                "name": "main_window",
                "width": 500,
                "height": 500
            },
            "image": {
                "src": "Images/Sun.png",
                "name": "sun1",
                "hOffset": 250,
                "vOffset": 250,
                "alignment": "center"
            },
            "text": {
                "data": "Click Here",
                "size": 36,
                "style": "bold",
                "name": "text1",
                "hOffset": 250,
                "vOffset": 100,
                "alignment": "center",
                "onMouseUp": "sun1.opacity = (sun1.opacity / 100) * 90;"
            }
        }})");

    std::cout << "widget window title: " << w["widget"]["window"]["title"].get_string() << std::endl;
    std::cout << "widget window name: " << w["widget"]["window"]["name"].get_string() << std::endl;
    std::cout << "widget image name: " << w["widget"]["image"]["name"].get_string() << std::endl;
    std::cout << "widget image name: " << w["widget"]["image"]["hOffset"].get_integer() << std::endl;
    std::cout << "widget text onMouseUp: " << w["widget"]["text"]["onMouseUp"].get_string() << std::endl;
  
# How to build
   1. Git clone or download the sourc
   2. Open "x64 Native Tools Command Prompt"
   3. Natigate to the source code folder
   4. Run "code ." to launch VS Code
   5. Ctrl-Shift+ B to build sample
    
