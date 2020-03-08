#pragma once

#include "syntax.hpp"



Token::Token() {}

Token::Token(Byte type) {
    this->type = type;
}

Token::Token(std::string string) {
    this->string = string;
}

Token::Token(Byte type, std::string string) {
    this->type = type;
    this->string = string;
}

Token::Token(Byte type, std::string string, int index) {
    this->type = type;
    this->string = string;
    this->index = index;
}

// Index won't be compared.
bool Token::compare(Token token) {
    return (this->type == token.type && this->string == token.string);
}

Token Token::getCloseParen() {
    switch (this->type) {
        case TK_LeftParen:
        return Token(TK_RightParen, ")", this->index);

        case TK_LeftBracket:
        return Token(TK_RightBracket, "]", this->index);

        case TK_LeftBrace:
        return Token(TK_RightBrace, "}", this->index);

        case TK_LeftAngleBracket:
        return Token(TK_RightAngleBracket, ">", this->index);
    }

    return Token();
}

Token Token::getOpenParen() {
    switch (this->type) {
        case TK_RightParen:
        return Token(TK_LeftParen, "(", this->index);

        case TK_RightBracket:
        return Token(TK_LeftBracket, "[", this->index);

        case TK_RightBrace:
        return Token(TK_LeftBrace, "{", this->index);

        case TK_RightAngleBracket:
        return Token(TK_LeftAngleBracket, "<", this->index);
    }

    return Token();
}

std::pair<int, int> Token::getPosition(std::string source) {
    int line = 0;
    int pos = 0;

    for(int i = 0; i < this->index; i++) {
        if(source[i] == '\n') {
            pos = 0;
            line++;
        } else {
            pos++;
        }
    }

    return { line, pos };
}

std::string Token::getPositionText(std::string filePath, std::string source) {
    std::pair pos = this->getPosition(source);
    return filePath + ":" + std::to_string(pos.first + 1) + ":" + std::to_string(pos.second + 1);
}

bool Token::isParen() {
    return this->match(ByteSeq { TK_LeftParen, TK_LeftBracket, TK_LeftBrace, TK_RightParen, TK_RightBracket, TK_RightBrace });
}

bool Token::isCloseParen() {
    return this->match(ByteSeq { TK_RightParen, TK_RightBracket, TK_RightBrace });
}

bool Token::isOpenParen() {
    return this->match(ByteSeq { TK_LeftParen, TK_LeftBracket, TK_LeftBrace });
}

bool Token::match(ByteSeq matches) {
    bool res = false;

    for(int i = 0; i < matches.size(); i++) {
        if(matches[i] == this->type) {
            res = true;
        }
    }

    return res;
}

bool Token::match(std::string regexp) {
    return std::regex_match(this->string, std::regex(regexp));
}



Node::Node() {}

Node::Node(Byte type) {
    this->type = type;
}

Node::Node(Byte type, std::vector<Node> children) {
    this->type = type;
    this->children = children;
}

Node::Node(Byte type, std::vector<Token> tokens) {
    this->type = type;
    this->tokens = tokens;
}

Node::Node(Byte type, std::vector<Node> children, std::vector<Token> tokens) {
    this->type = type;
    this->children = children;
    this->tokens = tokens;
}

void Node::addChild(Node node) {
    this->children.push_back(node);
}

// トークン配列, 呼び出し元のパーサ(ポインタ)
//Node::addChild(std::vector<Token> tokens, Parser* parser) {
    //if(tokens.size() >= 1) this->addChild(parser->getNode(tokens));
    //else this->addChild(Node(N_TOKEN, tokens));
//}

void Node::addToken(Token token) {
    this->tokens.push_back(token);
}

Node Node::childAt(int index) {
    return this->children[(index >= 0 ? index : this->children.size() + index)];
}

Token Node::tokenAt(int index) {
    return this->tokens[(index >= 0 ? index : this->tokens.size() + index)];
}

void Node::print() {
    this->print(this->prefix);
}

void Node::print(std::string level) {
    std::cout << "\033[44m" << level << "\033[m" << " " << "\033[94m" << this->typeToString() << "\033[m" << std::endl;

    for(Token token : this->tokens)
        std::cout << "\033[44m" << level << this->prefix << "\033[m" << " " << token.string << std::endl;

    for(Node node : this->children)
        node.print(level + this->prefix);
}

std::string Node::typeToString() {
    if(nodeTypeMap.count(this->type) == 1)
        return nodeTypeMap[this->type];
    else
        return "???";
}
