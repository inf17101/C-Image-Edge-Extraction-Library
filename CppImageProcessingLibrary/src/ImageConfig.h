#pragma once

#include <string>

class ImageProperty
{
    public:
        ImageProperty(const std::string& name) : name_(name) {}
        virtual ~ImageProperty() = default;
        const std::string getPropertyName() const { return name_; }
    private:
        std::string name_;
};

template<typename T>
class TypedImageProperty : public ImageProperty
{
    public:
        TypedImageProperty(const std::string& name, const T& data) : ImageProperty(name), data_(data) {}
        T getProperty() const { return data_; }
    private:
        T data_;

};