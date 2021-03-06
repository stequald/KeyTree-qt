////////////////////////////////////////////////////////////////////////////////
//
// keytreeutil.h
//
// Copyright (c) 2013-2014 Tim Lee
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "keytreeutil.h"
#include <sstream>

namespace KeyTreeUtil {

    uchar_vector extKeyBase58OrHexToBytes(const std::string& extKey) {
        uchar_vector extendedKey;
        if (isBase58CheckValid(extKey))
            extendedKey = fromBase58ExtKey(extKey);
        else if (StringUtils::isHex(extKey))
            extendedKey = uchar_vector(extKey);
        else
            throw std::runtime_error("Invalid extended key. Extended key must be in base58 or hex form.");

        return extendedKey;
    }

    uchar_vector fromBase58ExtKey(const std::string& extKey) {
        static unsigned int dummy = 0;
        uchar_vector fillKey;
        fromBase58Check(extKey, fillKey, dummy);
        static const std::string VERSION_BYTE("04");
        return uchar_vector(VERSION_BYTE+fillKey.getHex()); //append VERSION_BYTE to begining
    }

    TreeChains parseChainString(const std::string& chainStr, bool isPrivate) {
        TreeChains treeChains;

        const std::string s = StringUtils::split(chainStr)[0]; //trim trailing whitespaces

        std::deque<std::string> splitChain = StringUtils::split(s, '/');

        if (splitChain[0] != "m")
            throw std::runtime_error("Invalid Chain string.");

        //account for root node
        treeChains.push_back(IsPrivateNPathRange(true , Range(NODE_IDX_M, NODE_IDX_M)));

        if (splitChain.back() == "") splitChain.pop_back(); // happens if chainStr has '/' at end
        splitChain.pop_front();
        for (std::string& node : splitChain) {
            if (node.back() == '\'') {
                if (! isPrivate) throw std::runtime_error("Invalid chain "+ chainStr+ ",  not private extended key.");

                node = node.substr(0,node.length()-1);
                if (node.front() == '(' && node.back() == ')') {
                    IsPrivateNPathRange range = parseRange(node, true);
                    treeChains.push_back(range);
                } else {
                    uint32_t num = toPrime(std::stoi(node));
                    if (num == NODE_IDX_M)
                        throw std::invalid_argument("Invalid arguments. Invalid Range: " + std::to_string(num));
                    treeChains.push_back(IsPrivateNPathRange(true , Range(num, num)));
                }
            } else {
                if (node.front() == '(' && node.back() == ')') {
                    IsPrivateNPathRange range = parseRange(node, false);
                    treeChains.push_back(range);
                } else {
                    uint32_t num = std::stoi(node);
                    if (num == NODE_IDX_M)
                        throw std::invalid_argument("Invalid arguments. Invalid Range: " + std::to_string(num));
                    treeChains.push_back(IsPrivateNPathRange(false , Range(num, num)));
                }
            }
        }

        return treeChains;
    }

    IsPrivateNPathRange parseRange(const std::string node, bool isPrivate) {
        //node must be like (123-9345)
        const std::string minMaxString =  node.substr(1,node.length()-2);
        const std::deque<std::string> minMaxPair = StringUtils::split(minMaxString, '-');

        if (minMaxPair.size() != 2)
            throw std::invalid_argument("Invalid arguments.");

        uint32_t min = std::stoi(minMaxPair[0]);
        uint32_t max = std::stoi(minMaxPair[1]);
        if (min == NODE_IDX_M)
            throw std::invalid_argument("Invalid arguments. Invalid Range: " + std::to_string(min));
        if (max == NODE_IDX_M)
            throw std::invalid_argument("Invalid arguments. Invalid Range: " + std::to_string(max));

        if (isPrivate) {
            return IsPrivateNPathRange(true, Range(min, max));
        } else {
            return IsPrivateNPathRange(false, Range(min, max));
        }
    }

    std::string iToString(uint32_t i) {
        std::stringstream ss;
        ss << (0x7fffffff & i);
        if (isPrime(i)) { ss << "'"; }
        return ss.str();
    }

}
