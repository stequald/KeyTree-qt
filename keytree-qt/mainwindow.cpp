////////////////////////////////////////////////////////////////////////////////
//
// mainwindow.cpp
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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include "keynode/logger.h"
#include "keynode/CoinClasses/Base58Check.h"

using namespace std;

static const std::string HELP = "help";
static const std::string SEED_FORMAT = "seed_format";

static const std::string SEED = "seed";
static const std::string SEED_HEX = "seed.hex";
static const std::string EXTENDEDKEY = "extkey";
static const std::string CHAIN = "chain";
static const std::string I_MIN = "imin";
static const std::string I_MAX = "imax";
static const std::string TREE_TRAVERSAL_OPTION = "traverse";
static const std::string TREE_TRAVERSAL_TYPE_PREORDER = "preorder";
static const std::string TREE_TRAVERSAL_TYPE_POSTORDER = "postorder";
static const std::string TREE_TRAVERSAL_TYPE_LEVELORDER = "levelorder";
static const std::string VERBOSE_OPTION = "verbose";

static const std::string SEED_SHORT = "s";
static const std::string SEED_SHORT_HEX_SHORT = "s.h";
static const std::string SEED_HEX_SHORT = "seed.h";
static const std::string SEED_SHORT_HEX = "s.hex";

static const std::string EXTENDEDKEY_SHORT = "ek";
static const std::string CHAIN_SHORT = "c";
static const std::string I_MIN_SHORT = "min";
static const std::string I_MAX_SHORT = "max";
static const std::string TREE_TRAVERSAL_OPTION_SHORT = "trav";
static const std::string TREE_TRAVERSAL_TYPE_PREORDER_SHORT = "pre";
static const std::string TREE_TRAVERSAL_TYPE_POSTORDER_SHORT = "post";
static const std::string TREE_TRAVERSAL_TYPE_LEVELORDER_SHORT = "lev";
static const std::string VERBOSE_OPTION_SHORT = "v";

static const std::string cmdName = "./kt";
static const std::string exampleArg1 = " -seed \"correct horse battery staple\" -chain \"m/0'/0\"";
static const std::string exampleArg2 = " -seed.hex 000102030405060708090a0b0c0d0e0f -c \"m/0'/0\"";
static const std::string exampleArg3 = " -s.hex 000102030405060708090a0b0c0d0e0f -chain \"m/0'/0\"";
static const std::string exampleArg4 = " -s.h 000102030405060708090a0b0c0d0e0f -c \"m/0'/0\"";
static const std::string exampleArg5 = " -ek \"xprv9uHRZZhk6KAJC1avXpDAp4MDc3sQKNxDiPvvkX8Br5ngLNv1TxvUxt4cV1rGL5hj6KCesnDYUhd7oWgT11eZG7XnxHrnYeSvkzY7d2bhkJ7\" -c \"m/0'/0\"";
static const std::string exampleArg6 = " -extkey \"xpub68Gmy5EdvgibQVfPdqkBBCHxA5htiqg55crXYuXoQRKfDBFA1WEjWgP6LHhwBZeNK1VTsfTFUHCdrfp1bgwQ9xv5ski8PX9rL2dZXvgGDnw\" -chain \"m/0/0\"";
static const std::string exampleArg7 = " -ek \"xprv9uHRZZhk6KAJC1avXpDAp4MDc3sQKNxDiPvvkX8Br5ngLNv1TxvUxt4cV1rGL5hj6KCesnDYUhd7oWgT11eZG7XnxHrnYeSvkzY7d2bhkJ7\" -imin 0 -imax 3";
static const std::string exampleArg8 = " -extkey \"xpub68Gmy5EdvgibQVfPdqkBBCHxA5htiqg55crXYuXoQRKfDBFA1WEjWgP6LHhwBZeNK1VTsfTFUHCdrfp1bgwQ9xv5ski8PX9rL2dZXvgGDnw\" -min 0 -max 3";

static const std::string exampleArg9 = " -extkey \"xprv9uHRZZhk6KAJC1avXpDAp4MDc3sQKNxDiPvvkX8Br5ngLNv1TxvUxt4cV1rGL5hj6KCesnDYUhd7oWgT11eZG7XnxHrnYeSvkzY7d2bhkJ7\"";
static const std::string exampleArg10 = " -ek \"xpub68Gmy5EdvgibQVfPdqkBBCHxA5htiqg55crXYuXoQRKfDBFA1WEjWgP6LHhwBZeNK1VTsfTFUHCdrfp1bgwQ9xv5ski8PX9rL2dZXvgGDnw\"";

static const std::string exampleArg11 = " -seed.hex \"000102030405060708090a0b0c0d0e0f\" -chain \"m/0'/(3-6)'/(1-2)/8\"";
static const std::string exampleArg12 = " -extkey \"xprv9uHRZZhk6KAJC1avXpDAp4MDc3sQKNxDiPvvkX8Br5ngLNv1TxvUxt4cV1rGL5hj6KCesnDYUhd7oWgT11eZG7XnxHrnYeSvkzY7d2bhkJ7\" -chain \"m/0'/(5-8)'\"";

static const std::string exampleArg13 = " -ek \"xpub68Gmy5EdvgibQVfPdqkBBCHxA5htiqg55crXYuXoQRKfDBFA1WEjWgP6LHhwBZeNK1VTsfTFUHCdrfp1bgwQ9xv5ski8PX9rL2dZXvgGDnw\" -chain \"m/0/(3-4)/(1-2)\" -traverse levelorder";
static const std::string exampleArg14 = " -seed.hex \"000102030405060708090a0b0c0d0e0f\" -chain \"m/0'/(3-4)'/6'\" -trav postorder";

static const std::string exampleArg15 = " -verbose -s.h \"000102030405060708090a0b0c0d0e0f\" -chain \"m/0'/(3-4)'/6'\"";
static const std::string exampleArg16 = " -v -ek \"xprv9uHRZZhk6KAJC1avXpDAp4MDc3sQKNxDiPvvkX8Br5ngLNv1TxvUxt4cV1rGL5hj6KCesnDYUhd7oWgT11eZG7XnxHrnYeSvkzY7d2bhkJ7\"";
static const std::string CUSTOM_CHAIN = "Custom";
static const std::string NO_CHAIN = "None";
static const std::string DEFAULT_I_MIN = "0";
static const std::string DEFAULT_I_MAX = "4";

static const std::vector<std::string> defaultChains({NO_CHAIN,
                                                     CUSTOM_CHAIN,
                                                     "m/0'/(0-1)",
                                                     "m/0'/0",
                                                     "m/1'/1",
                                                     "m/0/2"});

void MainWindow::outputExtKeysFromSeed(const std::string& seed, const std::string& chainStr,
                           StringUtils::StringFormat seedStringFormat, TreeTraversal::Type traversalType,
                           bool isVerbose) {
    std::string seedHex;
    if (seedStringFormat == StringUtils::ascii) {
        seedHex = StringUtils::string_to_hex(seed);

    } else if (seedStringFormat == StringUtils::hex) {
        if (! StringUtils::isHex(seed))
            throw std::runtime_error("Invalid hex string \"" + seed + "\"");

        seedHex = seed;
    } else throw std::runtime_error("Invalid seed string format.");

    KeyNodeSeed keyNodeSeed((uchar_vector(seedHex)));
    bytes_t k = keyNodeSeed.getMasterKey();
    bytes_t c = keyNodeSeed.getMasterChainCode();

    KeyNode prv(k, c);
    TreeChains treeChains = parseChainString(chainStr, prv.isPrivate());
    outputString("Master (hex): " + seedHex);

    if (traversalType == TreeTraversal::postorder)
        traversePostorder(prv, treeChains, "m", isVerbose, NULL);
    else if (traversalType == TreeTraversal::levelorder) {
        treeChains.pop_front();
        std::deque<KeyNode> KeyNodeDeq;
        std::deque<std::pair<uint64_t,std::string>> levelNChainDeq;
        std::deque<Node*> graphNodeDeq;
        traverseLevelorder(prv, treeChains, "m", 0, KeyNodeDeq, levelNChainDeq,
                           graphNodeDeq, NULL, isVerbose);
    }
    else
        traversePreorder(prv, treeChains, "m", isVerbose, NULL);
}

void MainWindow::outputExtKeysFromExtKey(const std::string& extKey, const std::string& chainStr,
                             TreeTraversal::Type traversalType, const bool isVerbose) {
    uchar_vector extendedKey(extKeyBase58OrHexToBytes(extKey));
    KeyNode keyNode(extendedKey);
    TreeChains treeChains = parseChainString(chainStr, keyNode.isPrivate());
    if (isVerbose) outputExtraKeyNodeData(keyNode);

    if (traversalType == TreeTraversal::postorder)
        traversePostorder(keyNode, treeChains, "___", isVerbose);
    else if (traversalType == TreeTraversal::levelorder) {
        treeChains.pop_front();
        std::deque<KeyNode> KeyNodeDeq;
        std::deque<std::pair<uint64_t,std::string>> levelNChainDeq;
        std::deque<Node*> graphNodeDeq;
        traverseLevelorder(keyNode, treeChains, "___", 0, KeyNodeDeq, levelNChainDeq,
                           graphNodeDeq, NULL, isVerbose);
    } else
        traversePreorder(keyNode, treeChains, "___", isVerbose);
}

void MainWindow::outputKeyAddressofExtKey(const std::string& extKey, bool isVerbose) {
    uchar_vector extendedKey(extKeyBase58OrHexToBytes(extKey));
    KeyNode keyNode(extendedKey);
    visit(keyNode, "___", true);
    if (isVerbose) outputExtraKeyNodeData(keyNode);
    outputString("");
}

void MainWindow::traversePreorder(const KeyNode& keyNode, TreeChains treeChains,
                                  const std::string& chainName, bool isVerbose, Node* currentLeft) {
    if (! treeChains.empty()) {
        IsPrivateNPathRange isPrivateNPathRange = treeChains.front();
        treeChains.pop_front();
        bool isPrivate = isPrivateNPathRange.first;
        Range range = isPrivateNPathRange.second;
        uint32_t min = range.first;
        uint32_t max = range.second;

        if (min == NODE_IDX_M && max == NODE_IDX_M) {
            visit(keyNode, "m", isVerbose);
           traversePreorder(keyNode, treeChains, chainName, isVerbose);
        } else {
            for (uint32_t i = min; i <= max; ++i) {
                uint32_t k = i;
                if (isPrivate) k = toPrime(k);
                std::string childChainName = chainName + "/" + iToString(k);
                KeyNode childNode = keyNode.getChild(k);
                Node* leaf = visit(childNode, childChainName, isVerbose, currentLeft);
                traversePreorder(childNode, treeChains, childChainName, isVerbose, leaf);
            }
        }
    }
}

void MainWindow::traversePostorder(const KeyNode& keyNode, TreeChains treeChains,
                                   const std::string& chainName, bool isVerbose, Node* currentLeft) {
    if (! treeChains.empty()) {
        IsPrivateNPathRange isPrivateNPathRange = treeChains.front();
        treeChains.pop_front();
        bool isPrivate = isPrivateNPathRange.first;
        Range range = isPrivateNPathRange.second;
        uint32_t min = range.first;
        uint32_t max = range.second;

        if (min == NODE_IDX_M && max == NODE_IDX_M) {
            std::string nodeData(this->getNodeDataString(keyNode, "m", isVerbose));
            QString nodeDescription =  this->qStringFromSTDString(nodeData);
            Node* leaf = this->treeWidget->addItem(nodeDescription, currentLeft);
            traversePostorder(keyNode, treeChains, chainName, isVerbose, leaf);
            outputString(nodeData);
        } else {
            for (uint32_t i = min; i <= max; ++i) {
                uint32_t k = i;
                if (isPrivate) k = toPrime(k);
                std::string childChainName = chainName + "/" + iToString(k);
                KeyNode childNode = keyNode.getChild(k);

                std::string nodeData(this->getNodeDataString(childNode, childChainName, isVerbose));
                QString nodeDescription =  this->qStringFromSTDString(nodeData);
                Node* leaf = this->treeWidget->addItem(nodeDescription, currentLeft);
                traversePostorder(childNode, treeChains, childChainName, isVerbose, leaf);
                outputString(nodeData);
            }
        }
    }
}

void MainWindow::traverseLevelorder(const KeyNode& keyNode, const TreeChains& treeChains,
                                    const std::string& chainName, uint64_t level,
                                    std::deque<KeyNode>& keyNodeDeq,
                                    std::deque<std::pair<uint64_t,std::string>>& levelNChainDeq,
                                    std::deque<Node*>& graphNodeDeq, Node* leaf,
                                    bool isVerbose) {

    uint32_t childCount = 0;
    if (level < treeChains.size()) {
        IsPrivateNPathRange isPrivateNPathRange = treeChains.at(level);
        bool isPrivate = isPrivateNPathRange.first;
        Range range = isPrivateNPathRange.second;
        uint32_t min = range.first;
        uint32_t max = range.second;

        level++;
        for (uint32_t i = min; i <= max; ++i) {
            uint32_t k = i;
            if (isPrivate) k = toPrime(k);
            std::string childChainName = chainName + "/" + iToString(k);
            KeyNode childNode = keyNode.getChild(k);

            keyNodeDeq.push_back(childNode);
            levelNChainDeq.push_back(std::pair<uint64_t,std::string>(level,childChainName));

            childCount++;
        }
    }

    Node* newLeaf = visit(keyNode, chainName, isVerbose, leaf);
    for (uint32_t i = 0; i < childCount; ++i) {
        graphNodeDeq.push_back(newLeaf);
    }

    if (! keyNodeDeq.empty()) {
        std::pair<uint64_t,std::string> pair = levelNChainDeq.front();
        uint64_t lev = pair.first++;
        std::string cc = pair.second;
        KeyNode node = keyNodeDeq.front();
        keyNodeDeq.pop_front();
        levelNChainDeq.pop_front();

        Node*leaf = graphNodeDeq.front();
        graphNodeDeq.pop_front();

        traverseLevelorder(node, treeChains, cc, lev, keyNodeDeq, levelNChainDeq,
                           graphNodeDeq, leaf, isVerbose);
    }
}

Node* MainWindow::visit(const KeyNode& keyNode, const std::string& chainName,
                        bool isVerbose, Node* currentLeft) {
    std::string nodeData(this->getNodeDataString(keyNode, chainName, isVerbose));
    outputString(nodeData);
    QString nodeDescription =  this->qStringFromSTDString(nodeData);
    Node* leaf = this->treeWidget->addItem(nodeDescription, currentLeft);
    return leaf;
}

std::string MainWindow::getNodeDataString(const KeyNode& keyNode, const std::string& chainName,
                                          bool isVerbose) {
    std::string nodeData("");
    nodeData += "* [Chain " + chainName + "]\n";
    if (keyNode.isPrivate()) {
        KeyNode keyNodePub= keyNode.getPublic();
        nodeData += "  * ext pub:  " + toBase58Check(keyNodePub.extkey()) + "\n";
        nodeData += "  * ext prv:  " + toBase58Check(keyNode.extkey())+ "\n";
        nodeData += "  * priv key: " + keyNode.privkey() + "\n";
        nodeData += "  * address:  " + keyNode.address();
        if (isVerbose) {
            nodeData += "\n  * pub key:  " + toBase58Check(keyNode.pubkey()) + "\n";
        } else nodeData += "\n";
    } else {
        nodeData += "  * ext pub:  " + toBase58Check(keyNode.extkey()) + "\n";
        nodeData += "  * address:  " + keyNode.address();
        if (isVerbose) {
            nodeData += "\n  * pub key:  " + toBase58Check(keyNode.pubkey()) + "\n";
        } else nodeData += "\n";
    }
    return nodeData;
}

void MainWindow::outputExtraKeyNodeData(const KeyNode& keyNode) {
    outputString("  * depth:              " + std::to_string(keyNode.depth()));
    uint32_t childNum = keyNode.child_num();
    if (isPrime(childNum))
        outputString("  * child number:       " + std::to_string(removePrime(childNum))+"'");
    else
        outputString("  * child number:       " + std::to_string(childNum));
    std::stringstream stream;
    stream << std::hex << keyNode.parent_fp();
    std::string parent_fp(stream.str());
    outputString("  * parent fingerprint: " + parent_fp);
    std::stringstream stream2;
    stream2 << std::hex << keyNode.fp();
    std::string fp(stream2.str());
    outputString("  * fingerprint:        " + fp);
}

TreeChains MainWindow::parseChainString(const std::string& chainStr, bool isPrivate) {
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

IsPrivateNPathRange MainWindow::parseRange(const std::string node, bool isPrivate) {
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

std::string MainWindow::iToString(uint32_t i) {
    std::stringstream ss;
    ss << (0x7fffffff & i);
    if (isPrime(i)) { ss << "'"; }
    return ss.str();
}

uchar_vector MainWindow::extKeyBase58OrHexToBytes(const std::string& extKey) {
    uchar_vector extendedKey;
    if (isBase58CheckValid(extKey))
        extendedKey = fromBase58ExtKey(extKey);
    else if (StringUtils::isHex(extKey))
        extendedKey = uchar_vector(extKey);
    else
        throw std::runtime_error("Invalid extended key. Extended key must be in base58 or hex form.");

    return extendedKey;
}

uchar_vector MainWindow::fromBase58ExtKey(const std::string& extKey) {
    static unsigned int dummy = 0;
    uchar_vector fillKey;
    fromBase58Check(extKey, fillKey, dummy);
    static const std::string VERSION_BYTE("04");
    return uchar_vector(VERSION_BYTE+fillKey.getHex()); //append VERSION_BYTE to begining
}

void MainWindow::outputString(const std::string& str) {
    QString qtext = qStringFromSTDString(str+"");
    ui->resultsTextEdit->append(qtext);
}

void MainWindow::testVector1() {
    this->clearTree();
    outputExtKeysFromSeed("000102030405060708090a0b0c0d0e0f", "m/0'/1/2'/2/1000000000", StringUtils::StringFormat::hex);
}

void MainWindow::testVector1() {
    this->clearTree();
    outputExtKeysFromSeed("000102030405060708090a0b0c0d0e0f", "m/0'/1/2'/2/1000000000", StringUtils::StringFormat::hex);
}

void MainWindow::testVector2() {
    this->clearTree();
    std::string seed = "fffcf9f6f3f0edeae7e4e1dedbd8d5d2cfccc9c6c3c0bdbab7b4b1aeaba8a5a29f9c999693908d8a8784817e7b7875726f6c696663605d5a5754514e4b484542";
    outputExtKeysFromSeed(seed, "m/0/2147483647'/1/2147483646'/2", StringUtils::StringFormat::hex);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //QSize windowFixedSize(800, 800);
    //setFixedSize(windowFixedSize);
    this->setMinimumSize(100, 100);

    treeWidget = new GraphWidget;
    ui->formLayout->addWidget(treeWidget);

    connect( ui->extKeyButton, SIGNAL( clicked() ), this, SLOT(goClicked()) );
    connect( ui->seedLineEdit, SIGNAL( editingFinished() ), this, SLOT( seedEditingFinished()) );
    connect( ui->seedHexLineEdit, SIGNAL( editingFinished() ), this, SLOT( seedHexEditingFinished()) );
    connect( ui->extKeyLineEdit, SIGNAL( editingFinished() ), this, SLOT( extkeyEditingFinished()) );
    connect( ui->extKeyRadioButton, SIGNAL( clicked() ), this, SLOT( extKeyRadioButtonClicked()) );
    connect( ui->seedRadioButton, SIGNAL( clicked() ), this, SLOT( seedRadioButtonClicked()) );
    connect( ui->extKeyRadioButton, SIGNAL( clicked() ), this, SLOT( extKeyRadioButtonClicked()) );
    connect( ui->defaultChainsComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( defaultChainsComboBoxActivated(int)) );

    for(std::string chain : defaultChains) {
        ui->defaultChainsComboBox->addItem(qStringFromSTDString(chain));
    }

    ui->seedRadioButton->setChecked(true);
    seedRadioButtonClicked();
    fullTreeDescription = std::string("");
    ui->resultsTextEdit->setFont(QFont ("Helvetica", 8));

    testVector1();
}

void MainWindow::defaultChainsComboBoxActivated(int idx)
{
    QString chain(ui->defaultChainsComboBox->itemText(idx));
    std::string chainStr = stdStringFromQString(chain);
    if (chainStr == CUSTOM_CHAIN || chainStr == NO_CHAIN)
        ui->chainLineEdit->setText("");
    else
        ui->chainLineEdit->setText(chain);
}

void MainWindow::highlightBackgroundRed(QLineEdit* lineEdit)
{
    lineEdit->setStyleSheet("QLineEdit{background: red;}");

}

void MainWindow::unHighlightBackground(QLineEdit* lineEdit)
{
    lineEdit->setStyleSheet("QLineEdit{background: white;}");
}

void MainWindow::unHighlightAllTextEditsBackground()
{
    ui->extKeyLineEdit->setStyleSheet("QLineEdit{background: white;}");
    ui->seedLineEdit->setStyleSheet("QLineEdit{background: white;}");
    ui->seedHexLineEdit->setStyleSheet("QLineEdit{background: white;}");
    ui->chainLineEdit->setStyleSheet("QLineEdit{background: white;}");
}

void MainWindow::seedRadioButtonClicked()
{
    ui->extKeyLineEdit->setEnabled(false);
    ui->extKeyLineEdit->clear();

    ui->seedLineEdit->setEnabled(true);
    ui->seedHexLineEdit->setEnabled(true);
}

void MainWindow::extKeyRadioButtonClicked()
{
    ui->seedLineEdit->clear();
    ui->seedHexLineEdit->clear();
    ui->seedLineEdit->setEnabled(false);
    ui->seedHexLineEdit->setEnabled(false);

    ui->extKeyLineEdit->setEnabled(true);
}

void MainWindow::seedHexEditingFinished()
{
    ui->seedLineEdit->clear();
}

void MainWindow::seedEditingFinished()
{
    std::string seed =  stdStringFromQString(ui->seedLineEdit->text());
    Logger::debug("*seed: " + seed);
    std::string seedHex = StringUtils::string_to_hex(seed);

    ui->seedHexLineEdit->setText(qStringFromSTDString(seedHex));
}

void MainWindow::extkeyEditingFinished()
{
    if (ui->extKeyLineEdit->text().trimmed().isEmpty()) {
    }
    else {
    }
}


void MainWindow::clearTree()
{
    ui->resultsTextEdit->clear();
    treeWidget->removeAllItem();
    delete treeWidget;
    treeWidget = new GraphWidget;
    ui->formLayout->addWidget(treeWidget);
}

void MainWindow::goClicked()
{
    std::string chain =  stdStringFromQString(ui->chainLineEdit->text());

    if (ui->extKeyRadioButton->isChecked()) {
        std::string extKey =  stdStringFromQString(ui->extKeyLineEdit->text());

        if (! this->isValidExtKey(extKey)) {
            this->highlightBackgroundRed(ui->extKeyLineEdit);
            ui->resultsTextEdit->clear();
            outputString("Error: Invalid extKey");
            return;
        }
        this->unHighlightAllTextEditsBackground();
        this->clearTree();

        try {
            if (chain != "") {
                this->outputExtKeysFromExtKey(extKey, chain);
            } else {
                this->outputKeyAddressofExtKey(extKey);
            }
        } catch (const std::runtime_error& err) {
            outputString("Error: " + std::string(err.what()));
        }
    }
    else if (ui->seedRadioButton->isChecked()) {
        std::string seedHex =  stdStringFromQString(ui->seedHexLineEdit->text());
        this->clearTree();

        try {
            if (chain != "") {
                this->outputExtKeysFromSeed(seedHex, chain, StringUtils::hex);
            }
        } catch (const std::runtime_error& err) {
            outputString("Error: " + std::string(err.what()));
        }

    }
}

bool MainWindow::isValidExtKey(const std::string extKey)
{
    if (extKey.length() < 50) { //TODO: only tmp fix for invalid extKey
        return false;
    }

    return true;
}

MainWindow::~MainWindow()
{
    delete treeWidget;
    delete ui;
}

std::string MainWindow::stdStringFromQString(QString str) {
    std::string s = str.toUtf8().constData();
    return s;
}

QString MainWindow::qStringFromSTDString(std::string str) {
    QString qstr = QString::fromStdString(str);
    return qstr;
}
