#pragma once 

#include <string>

class IDocumentRenderer {
public:
    virtual ~IDocumentRenderer() = default;

    virtual std::string render(const std::string& text) const = 0;
};


class BaseDocumentRenderer : public IDocumentRenderer {
public:
    std::string normalize(const std::string& text) const {
        std::string newString = "";
        for(auto c : text) {
            newString += std::tolower(c);
        }
        return newString;
    }
};


class PlainTextRenderer : public BaseDocumentRenderer {
public:
    std::string render(const std::string& text) const override {
        std::string newText = "";
        newText += normalize(text);
        return newText;
    }
};


class HtmlRenderer : public BaseDocumentRenderer {
public:
    std::string render(const std::string& text) const override {
        std::string newText = "<html>\n<body>\n";
        newText += normalize(text);
        newText += "\n</body>\n</html>\n";
        return newText;
    }
};



class RendererDecorator : public IDocumentRenderer {
private:
    const IDocumentRenderer& m_renderer;

public:
    RendererDecorator(const IDocumentRenderer& ren) : m_renderer(ren) {}

    const IDocumentRenderer& getRenderer() const {
        return m_renderer;
    }

};


class HeaderDecorator : public RendererDecorator {
private:
    std::string m_text;

public:
    HeaderDecorator(std::string text, const IDocumentRenderer& ren) 
    : RendererDecorator(ren), m_text(text) {}

    std::string render(const std::string& text) const override {
        return m_text + "\n" + getRenderer().render(text);
    }
};


class LoggingDecorator : public RendererDecorator {
public:
    LoggingDecorator(const IDocumentRenderer& ren) : RendererDecorator(ren) {}

    std::string render(const std::string& text) const override {
        std::cout << "[LOG] Rendering document with content:\n" + text + "\n";
        std::string temp = getRenderer().render(text);
        std::cout << "[LOG] Result is: \n" + temp + "\n";
        return temp;
    }
};