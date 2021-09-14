#pragma once

#include <string>

class ImageProperty
{
    std::string name_;
    public:
        ImageProperty(const std::string& name) : name_(name) {}
        virtual ~ImageProperty() = default;
        std::string getPropertyName() const { return name_; }
};

template<typename T>
class TypedImageProperty : public ImageProperty
{
    T data_;
    public:
        TypedImageProperty(const std::string& name, const T& data) : ImageProperty(name), data_(data) {}
        T getProperty() const { return data_; }

};