#include <>










int main() {
    std::vector<int> v;
    while (true) {
        try {
            auto line = fn::input();
            auto args = fn::split(line, ' ');
            if(args[0] == "div") {

            } else if (args[0] == "show") {

            } else {

            } catch (std::out_of_range& e) {

            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
            }
            //trata o caso de faltar uma variavel
            
        }
    }
}