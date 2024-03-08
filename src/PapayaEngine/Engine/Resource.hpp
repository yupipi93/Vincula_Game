#pragma once
#include <glew.h>
#include <string>

namespace pap {

class Resource {
public:
	//explicit pap::Resource (std::string p) : path{p} {}
    virtual ~Resource() = default;

    std::string path;
private:
};

} // NAMESPACE