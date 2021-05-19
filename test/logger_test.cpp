#include <iostream>
#include "../glUI/logger.hpp"

int main(){

    LOGI << "This is information";
    LOGD << "This is Debug information";
    LOGW << "This is Warning";
    LOGE << "This is Error!!";

    std::cout << "------------------------------------------\n\n";

    const char str_test[] = "test const string ";
    LOGI << str_test;
    LOGD << str_test;

    return 0;
}
