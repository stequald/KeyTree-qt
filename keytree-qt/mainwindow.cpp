////////////////////////////////////////////////////////////////////////////////
//
// kt.h
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
#include "keytree/logger.h"

using namespace std;

static const std::string HELP = "help";

static const std::string SEED_FORMAT = "seed_format";

static const std::string SEED = "seed";
static const std::string SEED_HEX = "seed.hex";

static const std::string EXTENDEDKEY = "extkey";
static const std::string CHAIN = "chain";
static const std::string I_MIN = "imin";
static const std::string I_MAX = "imax";

static const std::string SEED_SHORT = "s";
static const std::string SEED_SHORT_HEX_SHORT = "s.h";
static const std::string SEED_HEX_SHORT = "seed.h";
static const std::string SEED_SHORT_HEX = "s.hex";

static const std::string EXTENDEDKEY_SHORT = "ek";
static const std::string CHAIN_SHORT = "c";
static const std::string I_MIN_SHORT = "min";
static const std::string I_MAX_SHORT = "max";

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


static const std::string CUSTOM_CHAIN = "Custom";
static const std::string DEFAULT_I_MIN = "0";
static const std::string DEFAULT_I_MAX = "4";


void MainWindow::outputExtKeys(KeyTree& keyTree) {
    while (! keyTree.isAtEndOfChain()) { //TODO: change to iterator instead
        KeyNode data = keyTree.getNextInChain();
        outputString("* [Chain " + data.chain + "]");
        outputString("  * ext pub: " + data.extpub);
        if (data.extprv != "") outputString("  * ext prv: " + data.extprv);
        //outputString("  * priv key: " + data.privkey);
        //outputString("  * address: " + data.address);
        //outputString("");
    }
}

void MainWindow::outputExtKeysFromSeed(const std::string seed, const std::string chainStr, StringUtils::StringFormat seedStringFormat) {
    std::string seedHex;
    if (seedStringFormat == StringUtils::StringFormat::ascii) {
        seedHex = StringUtils::string_to_hex(seed);

    } else if (seedStringFormat == StringUtils::StringFormat::hex) {
        if (! StringUtils::isHex(seed))
        throw std::runtime_error("Invalid hex string \"" + seed + "\"");

        seedHex = seed;
    } else throw std::runtime_error("Invalid seed string format.");



    KeyTree keyTree(seed, chainStr, seedStringFormat);
    KeyNode data = keyTree.getCurrentInChain();
    outputString("Master (hex): " + seedHex);
    outputString("* [Chain " + data.chain + "]");
    outputString("  * ext pub: " + data.extpub);
    outputString("  * ext prv: " + data.extprv);
    //outputString("  * priv key: " + data.privkey);
    //outputString("  * address: " + data.address);
    //outputString("");
    outputExtKeys(keyTree);
}

void MainWindow::outputExtKeysFromExtKey(const std::string extKey, const std::string chainStr) {
    KeyTree keyTree(extKey, chainStr);
    outputExtKeys(keyTree);
}

void MainWindow::outputKeyAddressesFromExtKey(const std::string extKey, uint32_t i_min, uint32_t i_max) {
    for (uint32_t i = i_min; i < i_max; i++ ) {

        Logger::debug("ssssssssss");
        Logger::debug("extKey: " + extKey);

        KeyNode data = KeyTree::getChildOfExtKey(extKey, i);
        //outputString("* [Chain " + data.chain + "]");
        //outputString("  * ext pub: " + data.extpub);
        //outputString("  * ext prv: " + data.extprv);
        if (data.privkey != "") outputString("  * priv key: " + data.privkey);
        outputString("  * address: " + data.address);
        outputString("");
    }
}

void MainWindow::outputKeyAddressofExtKey(const std::string extKey) {
    KeyTree keyTree(extKey, "m");
    KeyNode data = keyTree.getCurrentInChain();
    //outputString("* [Chain " + data.chain + "]");
    outputString("  * ext pub: " + data.extpub);
    if (data.extprv != "") outputString("  * ext prv: " + data.extprv);
    if (data.privkey != "") outputString("  * priv key: " + data.privkey);
    outputString("  * address: " + data.address);
    outputString("");
}


void MainWindow::testVector1() {
    outputExtKeysFromSeed("000102030405060708090a0b0c0d0e0f", "m/0'/1/2'/2/1000000000", StringUtils::StringFormat::hex);
}

void MainWindow::testVector2() {
    std::string seed = "fffcf9f6f3f0edeae7e4e1dedbd8d5d2cfccc9c6c3c0bdbab7b4b1aeaba8a5a29f9c999693908d8a8784817e7b7875726f6c696663605d5a5754514e4b484542";
    outputExtKeysFromSeed(seed, "m/0/2147483647'/1/2147483646'/2", StringUtils::StringFormat::hex);
}

void MainWindow::testOutputExtKeysFromSeed() {
    outputExtKeysFromSeed("000102030405060708090a0b0c0d0e0f", "m/0'/0", StringUtils::StringFormat::hex);
}

void MainWindow::testOutputExtKeysFromExtKey() {
    //outputExtKeysFromExtKey("0488ade4013442193e8000000047fdacbd0f1097043b78c63c20c34ef4ed9a111d980047ad16282c7ae623614100edb2e14f9ee77d26dd93b4ecede8d16ed408ce149b6cd80b0715a2d911a0afea", "m/0'/0"); //priv
    //outputExtKeysFromExtKey("0488b21e013442193e8000000047fdacbd0f1097043b78c63c20c34ef4ed9a111d980047ad16282c7ae6236141035a784662a4a20a65bf6aab9ae98a6c068a81c52e4b032c0fb5400c706cfccc56", "m/0'/0"); //pub  - cant do chain with ' on ext pubkey will throw except, do below
    //outputExtKeysFromExtKey("0488b21e013442193e8000000047fdacbd0f1097043b78c63c20c34ef4ed9a111d980047ad16282c7ae6236141035a784662a4a20a65bf6aab9ae98a6c068a81c52e4b032c0fb5400c706cfccc56", "m/0/0"); //pub

    outputExtKeysFromExtKey("xprv9uHRZZhk6KAJC1avXpDAp4MDc3sQKNxDiPvvkX8Br5ngLNv1TxvUxt4cV1rGL5hj6KCesnDYUhd7oWgT11eZG7XnxHrnYeSvkzY7d2bhkJ7", "m/0'/0"); //priv
    //outputExtKeysFromExtKey("xpub68Gmy5EdvgibQVfPdqkBBCHxA5htiqg55crXYuXoQRKfDBFA1WEjWgP6LHhwBZeNK1VTsfTFUHCdrfp1bgwQ9xv5ski8PX9rL2dZXvgGDnw", "m/0'/0"); //pub  - cant do chain with ' on ext pubkey will throw except, do below
    //outputExtKeysFromExtKey("xpub68Gmy5EdvgibQVfPdqkBBCHxA5htiqg55crXYuXoQRKfDBFA1WEjWgP6LHhwBZeNK1VTsfTFUHCdrfp1bgwQ9xv5ski8PX9rL2dZXvgGDnw", "m/0/0"); //pub
}

void MainWindow::testOutputKeyAddressesFromExtKey() {
    //outputKeyAddressesFromExtKey("0488ade4013442193e8000000047fdacbd0f1097043b78c63c20c34ef4ed9a111d980047ad16282c7ae623614100edb2e14f9ee77d26dd93b4ecede8d16ed408ce149b6cd80b0715a2d911a0afea", 0, 2); //priv
    //outputKeyAddressesFromExtKey("0488b21e013442193e8000000047fdacbd0f1097043b78c63c20c34ef4ed9a111d980047ad16282c7ae6236141035a784662a4a20a65bf6aab9ae98a6c068a81c52e4b032c0fb5400c706cfccc56", 0, 2); //pub

    MainWindow::outputKeyAddressesFromExtKey("xprv9uHRZZhk6KAJC1avXpDAp4MDc3sQKNxDiPvvkX8Br5ngLNv1TxvUxt4cV1rGL5hj6KCesnDYUhd7oWgT11eZG7XnxHrnYeSvkzY7d2bhkJ7", 0, 2); //priv
    //outputKeyAddressesFromExtKey("xpub68Gmy5EdvgibQVfPdqkBBCHxA5htiqg55crXYuXoQRKfDBFA1WEjWgP6LHhwBZeNK1VTsfTFUHCdrfp1bgwQ9xv5ski8PX9rL2dZXvgGDnw", 0, 2); //pub
}

void MainWindow::testOutputKeyAddressofExtKey() {
    //outputKeyAddressofExtKey("0488ade4013442193e8000000047fdacbd0f1097043b78c63c20c34ef4ed9a111d980047ad16282c7ae623614100edb2e14f9ee77d26dd93b4ecede8d16ed408ce149b6cd80b0715a2d911a0afea"); //priv
    //outputKeyAddressofExtKey("0488b21e013442193e8000000047fdacbd0f1097043b78c63c20c34ef4ed9a111d980047ad16282c7ae6236141035a784662a4a20a65bf6aab9ae98a6c068a81c52e4b032c0fb5400c706cfccc56"); //pub

    outputKeyAddressofExtKey("xprv9uHRZZhk6KAJC1avXpDAp4MDc3sQKNxDiPvvkX8Br5ngLNv1TxvUxt4cV1rGL5hj6KCesnDYUhd7oWgT11eZG7XnxHrnYeSvkzY7d2bhkJ7"); //priv
    outputKeyAddressofExtKey("xpub68Gmy5EdvgibQVfPdqkBBCHxA5htiqg55crXYuXoQRKfDBFA1WEjWgP6LHhwBZeNK1VTsfTFUHCdrfp1bgwQ9xv5ski8PX9rL2dZXvgGDnw"); //pub
}


void MainWindow::outputExamples() {
    outputString("Input parameters can be in hex or base58.");
    outputString("Here are some examples:");
    outputString("");

    outputString("Given Seed and Chain will output Child Extended Keys:");
    outputString(cmdName+exampleArg1);
    outputString(cmdName+exampleArg2);
    outputString(cmdName+exampleArg3);
    outputString(cmdName+exampleArg4);
    outputString("");

    outputString("Given Extended Key and Chain will output Child Extended Keys:");
    outputString(cmdName+exampleArg5);
    outputString(cmdName+exampleArg6);
    outputString("");

    outputString("Given Extended Key and range will output Private Keys and Addresses from child of Extended Key in given range:");
    outputString(cmdName+exampleArg7);
    outputString(cmdName+exampleArg8);
    outputString("");

    outputString("Given Extended Key will output Private Key and Address of Extended Key:");
    outputString(cmdName+exampleArg9);
    outputString(cmdName+exampleArg10);
    outputString("");
}


void MainWindow::outputString(const std::string str) {
    Logger::log(str);
    QString qtext = qStringFromSTDString(str+"");
    ui->resultsTextEdit->append(qtext);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //QSize windowFixedSize(800, 800);
    //setFixedSize(windowFixedSize);
    this->setMinimumSize(100, 100);


    connect( ui->extKeyButton, SIGNAL( clicked() ), this, SLOT(extkeyClicked()) );
    connect( ui->bitcoinKeyButton, SIGNAL( clicked() ), this, SLOT(bitcoinkeyClicked()) );

    connect( ui->seedLineEdit, SIGNAL( editingFinished() ), this, SLOT( seedEditingFinished()) );
    connect( ui->seedHexLineEdit, SIGNAL( editingFinished() ), this, SLOT( seedHexEditingFinished()) );
    connect( ui->extKeyLineEdit, SIGNAL( editingFinished() ), this, SLOT( extkeyEditingFinished()) );



    connect( ui->extKeyRadioButton, SIGNAL( clicked() ), this, SLOT( extKeyRadioButtonClicked()) );
    connect( ui->seedRadioButton, SIGNAL( clicked() ), this, SLOT( seedRadioButtonClicked()) );

    connect( ui->extKeyRadioButton, SIGNAL( clicked() ), this, SLOT( extKeyRadioButtonClicked()) );


    connect( ui->defaultChainsComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( defaultChainsComboBoxActivated(int)) );


    QString defaultChain0 = qStringFromSTDString(CUSTOM_CHAIN);
    QString defaultChain1 = qStringFromSTDString("m/0'/0");
    QString defaultChain2 = qStringFromSTDString("m/0'/0");
    QString defaultChain3 = qStringFromSTDString("m/0'/0");

    ui->defaultChainsComboBox->addItem(defaultChain0);
    ui->defaultChainsComboBox->addItem(defaultChain1);
    ui->defaultChainsComboBox->addItem(defaultChain2);
    ui->defaultChainsComboBox->addItem(defaultChain3);

    ui->iMaxLineEdit->setValidator(new QIntValidator(0, INT_MAX, this));
    ui->iMinLineEdit->setValidator(new QIntValidator(0, INT_MAX, this));
    ui->seedRadioButton->setChecked(true);
    seedRadioButtonClicked();

    testVector1();
}

void MainWindow::defaultChainsComboBoxActivated(int idx)
{
    QString chain(ui->defaultChainsComboBox->itemText(idx));
    if (stdStringFromQString(chain)  == CUSTOM_CHAIN)
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

    ui->bitcoinKeyButton->setEnabled(false);

    ui->bitcoinKeyButton->setEnabled(false);

}

void MainWindow::extKeyRadioButtonClicked()
{
    ui->seedLineEdit->clear();
    ui->seedHexLineEdit->clear();
    ui->seedLineEdit->setEnabled(false);
    ui->seedHexLineEdit->setEnabled(false);

    ui->extKeyLineEdit->setEnabled(true);


    ui->bitcoinKeyButton->setEnabled(true);

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
        ui->bitcoinKeyButton->setEnabled(false);
    }
    else if(0) {

    }
    else {
        ui->bitcoinKeyButton->setEnabled(true);
    }
}



void MainWindow::extkeyClicked()
{
    std::string imin =  stdStringFromQString(ui->iMinLineEdit->text());
    std::string imax =  stdStringFromQString(ui->iMaxLineEdit->text());
    std::string chain =  stdStringFromQString(ui->chainLineEdit->text());

    Logger::debug("*imin: " + imin);
    Logger::debug("*imax: " + imax);
    Logger::debug("*chain: " + chain);

    if (ui->extKeyRadioButton->isChecked()) {
        std::string extKey =  stdStringFromQString(ui->extKeyLineEdit->text());
        Logger::debug("*extKey: " + extKey);


        if (! this->isValidExtKey(extKey)) {
            this->highlightBackgroundRed(ui->extKeyLineEdit);

            ui->resultsTextEdit->clear();
            outputString("Error: Invalid extKey");
            return;
        }
        this->unHighlightAllTextEditsBackground();
        ui->resultsTextEdit->clear();

        try {


            this->outputExtKeysFromExtKey(extKey, chain);
        } catch (const std::runtime_error& err) {
            outputString("Error: " + std::string(err.what()));
        }



    }
    else if (ui->seedRadioButton->isChecked()) {
        std::string seed =  stdStringFromQString(ui->seedLineEdit->text());
        std::string seedHex =  stdStringFromQString(ui->seedHexLineEdit->text());
        Logger::debug("*seed: " + seed);
        Logger::debug("*seedHex: " + seedHex);

        ui->resultsTextEdit->clear();
        try {
            this->outputExtKeysFromSeed(seedHex, chain, StringUtils::hex);
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

void MainWindow::bitcoinkeyClicked()
{
    std::string extKey =  stdStringFromQString(ui->extKeyLineEdit->text());

    if (! this->isValidExtKey(extKey)) {
        this->highlightBackgroundRed(ui->extKeyLineEdit);

        ui->resultsTextEdit->clear();
        outputString("Error: Invalid extKey");
        return;
    }
    this->unHighlightAllTextEditsBackground();


    if (ui->iMinLineEdit->text().trimmed().isEmpty()) {
        QString imin = qStringFromSTDString(DEFAULT_I_MIN);
        ui->iMinLineEdit->setText(imin);
    }
    if (ui->iMaxLineEdit->text().trimmed().isEmpty()) {
        QString imax = qStringFromSTDString(DEFAULT_I_MAX);
        ui->iMaxLineEdit->setText(imax);
    }

    std::string imin =  stdStringFromQString(ui->iMinLineEdit->text());
    std::string imax =  stdStringFromQString(ui->iMaxLineEdit->text());

    ui->resultsTextEdit->clear();

    try {
        Logger::debug("imin: " + imin);

        Logger::debug("imax: " + imin);
        std::cout << std::atoi(imin.c_str()) << endl;
        Logger::debug("asddfasddf");
        this->outputKeyAddressesFromExtKey(extKey, std::atoi(imin.c_str()), std::atoi(imax.c_str()));
        Logger::debug("asddfasddf");

    } catch (const std::runtime_error& err) {
        outputString("Error: " + std::string(err.what()));
    }

}




MainWindow::~MainWindow()
{
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
