#include "keytreeeditdialog.h"
#include "ui_keytreeeditdialog.h"
#include <sstream>
#include "qdebug.h"
#include "keynode/logger.h"
#include "guiutil.h"

static const QColor ROOT_NODE_PRIMARY_COLOR = Qt::blue;
static const QColor ROOT_NODE_SECONDARY_COLOR = Qt::darkBlue;
static const QColor NODE_PRIMARY_COLOR = Qt::yellow;
static const QColor NODE_SECONDARY_COLOR = Qt::darkYellow;
static const QColor LEAF_NODE_PRIMARY_COLOR = Qt::green;
static const QColor LEAF_NODE_SECONDARY_COLOR = Qt::darkGreen;

KeyTreeEditDialog::KeyTreeEditDialog(const KeyNode& keyNode, TreeChains& treeChains, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyTreeEditDialog)
{
    ui->setupUi(this);

    treeGraphWidget = new GraphWidget;
    ui->formLayout->addWidget(treeGraphWidget);

    ui->resultsTextEdit->setFont(QFont ("Helvetica", 8));

    ui->treeWidget->setColumnCount(1);

    ui->treeWidget->setHeaderLabels(QStringList() << "Level");

    connect( ui->exportButton, SIGNAL( clicked() ), this, SLOT(exportClicked()) );
    connect( ui->treeWidget, SIGNAL( itemSelectionChanged() ), this, SLOT(treeWidgetItemSelectionChanged()) );

    treeChains.pop_front();
    std::deque<KeyNode> KeyNodeDeq;
    std::deque<std::pair<uint64_t,std::string>> levelNChainDeq;
    std::deque<Node*> graphNodeDeq;
    traverseLevelorder(keyNode, treeChains, "m", 0, KeyNodeDeq, levelNChainDeq,
                       graphNodeDeq, NULL, false);

}

void KeyTreeEditDialog::treeWidgetItemSelectionChanged()
{
    Logger::info("treeWidgetItemSelectionChanged");
}

KeyTreeEditDialog::~KeyTreeEditDialog()
{
    delete treeGraphWidget;
    delete ui;
}

void KeyTreeEditDialog::exportClicked()
{
    QTreeWidgetItemIterator it(ui->treeWidget);
    while (*it)
    {
        QTreeWidgetItem* treeItem = *it;
        qDebug() << treeItem->text(0);
        ++it;
    }
}

void KeyTreeEditDialog::addKeyTreeLevel(QString level)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);
    treeItem->setText(0, level);
    treeItem->setFlags(treeItem->flags() | Qt::ItemIsUserCheckable);
    treeItem->setCheckState(0, Qt::Checked);
}

void KeyTreeEditDialog::addKeyTreeChild(QTreeWidgetItem *parent, QString chain)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setText(0, chain);
    treeItem->setFlags(treeItem->flags() | Qt::ItemIsUserCheckable);
    treeItem->setCheckState(0, Qt::Checked);
    parent->addChild(treeItem);
}

void KeyTreeEditDialog::buildTreeWidget(uint32_t level, const KeyNode& keyNode,
                                        const std::string& chainName)
{
    QString levelString =  GUIUtil::qStringFromSTDString(std::to_string(level));
    if (ui->treeWidget->topLevelItemCount() < level)
        addKeyTreeLevel(levelString);

    QTreeWidgetItem *treeItem = ui->treeWidget->topLevelItem(level-1);
    QString chainNameString =  GUIUtil::qStringFromSTDString(chainName);
    addKeyTreeChild(treeItem, chainNameString);
}

void KeyTreeEditDialog::traversePreorder(const KeyNode& keyNode, TreeChains treeChains,
                                  const std::string& chainName, bool isVerbose, Node* currentLeft) {
    if (! treeChains.empty()) {
        IsPrivateNPathRange isPrivateNPathRange = treeChains.front();
        treeChains.pop_front();
        bool isPrivate = isPrivateNPathRange.first;
        Range range = isPrivateNPathRange.second;
        uint32_t min = range.first;
        uint32_t max = range.second;

        if (min == KeyTreeUtil::NODE_IDX_M && max == KeyTreeUtil::NODE_IDX_M) {
            visit(keyNode, "m", isVerbose, ROOT_NODE_PRIMARY_COLOR, ROOT_NODE_SECONDARY_COLOR);
           traversePreorder(keyNode, treeChains, chainName, isVerbose);
        } else {
            for (uint32_t i = min; i <= max; ++i) {
                uint32_t k = i;
                if (isPrivate) k = KeyTreeUtil::toPrime(k);
                std::string childChainName = chainName + "/" + KeyTreeUtil::iToString(k);
                KeyNode childNode = keyNode.getChild(k);
                QColor nodePrimaryColor;
                QColor nodeSecondaryColor;
                 if (! treeChains.empty()) {
                     nodePrimaryColor = NODE_PRIMARY_COLOR;
                     nodeSecondaryColor = NODE_SECONDARY_COLOR;
                 }
                 else {
                     nodePrimaryColor = LEAF_NODE_PRIMARY_COLOR;
                     nodeSecondaryColor = LEAF_NODE_SECONDARY_COLOR;
                 }
                Node* leaf = visit(childNode, childChainName, isVerbose,
                                   nodePrimaryColor, nodeSecondaryColor, currentLeft);
                traversePreorder(childNode, treeChains, childChainName, isVerbose, leaf);
            }
        }
    }
}

void KeyTreeEditDialog::traversePostorder(const KeyNode& keyNode, TreeChains treeChains,
                                   const std::string& chainName, bool isVerbose, Node* currentLeft) {
    if (! treeChains.empty()) {
        IsPrivateNPathRange isPrivateNPathRange = treeChains.front();
        treeChains.pop_front();
        bool isPrivate = isPrivateNPathRange.first;
        Range range = isPrivateNPathRange.second;
        uint32_t min = range.first;
        uint32_t max = range.second;

        if (min == KeyTreeUtil::NODE_IDX_M && max == KeyTreeUtil::NODE_IDX_M) {
            std::string nodeData(this->getNodeDataString(keyNode, "m", isVerbose));
            QString nodeDescription =  GUIUtil::qStringFromSTDString(nodeData);
            Node* leaf = this->treeGraphWidget->addItem(nodeDescription, ROOT_NODE_PRIMARY_COLOR,
                                                   ROOT_NODE_SECONDARY_COLOR, currentLeft);
            traversePostorder(keyNode, treeChains, chainName, isVerbose, leaf);
            outputString(nodeData);
        } else {
            for (uint32_t i = min; i <= max; ++i) {
                uint32_t k = i;
                if (isPrivate) k = KeyTreeUtil::toPrime(k);
                std::string childChainName = chainName + "/" + KeyTreeUtil::iToString(k);
                KeyNode childNode = keyNode.getChild(k);

                QColor nodePrimaryColor;
                QColor nodeSecondaryColor;
                 if (! treeChains.empty()) {
                     nodePrimaryColor = NODE_PRIMARY_COLOR;
                     nodeSecondaryColor = NODE_SECONDARY_COLOR;
                 }
                 else {
                     nodePrimaryColor = LEAF_NODE_PRIMARY_COLOR;
                     nodeSecondaryColor = LEAF_NODE_SECONDARY_COLOR;
                 }
                std::string nodeData(this->getNodeDataString(childNode, childChainName, isVerbose));
                QString nodeDescription =  GUIUtil::qStringFromSTDString(nodeData);
                Node* leaf = this->treeGraphWidget->addItem(nodeDescription, nodePrimaryColor,
                                                       nodeSecondaryColor, currentLeft);
                traversePostorder(childNode, treeChains, childChainName, isVerbose, leaf);
                outputString(nodeData);
            }
        }
    }
}

void KeyTreeEditDialog::traverseLevelorder(const KeyNode& keyNode, const TreeChains& treeChains,
                                    const std::string& chainName, uint64_t level,
                                    std::deque<KeyNode>& keyNodeDeq,
                                    std::deque<std::pair<uint64_t,std::string>>& levelNChainDeq,
                                    std::deque<Node*>& graphNodeDeq, Node* leaf,
                                    bool isVerbose) {

    uint32_t childCount = 0;
    QColor nodePrimaryColor;
    QColor nodeSecondaryColor;
    if (level < treeChains.size()) {
        IsPrivateNPathRange isPrivateNPathRange = treeChains.at(level);
        bool isPrivate = isPrivateNPathRange.first;
        Range range = isPrivateNPathRange.second;
        uint32_t min = range.first;
        uint32_t max = range.second;

        level++;
        for (uint32_t i = min; i <= max; ++i) {
            uint32_t k = i;
            if (isPrivate) k = KeyTreeUtil::toPrime(k);
            std::string childChainName = chainName + "/" + KeyTreeUtil::iToString(k);
            KeyNode childNode = keyNode.getChild(k);

            keyNodeDeq.push_back(childNode);
            levelNChainDeq.push_back(std::pair<uint64_t,std::string>(level,childChainName));

            childCount++;
        }

        if (level != 1) {
            nodePrimaryColor = NODE_PRIMARY_COLOR;
            nodeSecondaryColor = NODE_SECONDARY_COLOR;
        } else {
            nodePrimaryColor = ROOT_NODE_PRIMARY_COLOR;
            nodeSecondaryColor = ROOT_NODE_SECONDARY_COLOR;
        }
        buildTreeWidget(level, keyNode, chainName);
    } else {
        nodePrimaryColor = LEAF_NODE_PRIMARY_COLOR;
        nodeSecondaryColor = LEAF_NODE_SECONDARY_COLOR;
        buildTreeWidget(level+1, keyNode, chainName);
    }

    Node* newLeaf = visit(keyNode, chainName, isVerbose, nodePrimaryColor, nodeSecondaryColor, leaf);

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

Node* KeyTreeEditDialog::visit(const KeyNode& keyNode, const std::string& chainName,
                        bool isVerbose, QColor nodePrimaryColor, QColor nodeSecondaryColor, Node* currentLeft) {
    std::string nodeData(this->getNodeDataString(keyNode, chainName, isVerbose));
    outputString(nodeData);
    QString nodeDescription =  GUIUtil::qStringFromSTDString(nodeData);
    Node* leaf = this->treeGraphWidget->addItem(nodeDescription,
                                           nodePrimaryColor, nodeSecondaryColor, currentLeft);
    return leaf;
}

std::string KeyTreeEditDialog::getNodeDataString(const KeyNode& keyNode, const std::string& chainName,
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

void KeyTreeEditDialog::outputExtraKeyNodeData(const KeyNode& keyNode) {
    outputString("  * depth:              " + std::to_string(keyNode.depth()));
    uint32_t childNum = keyNode.child_num();
    if (KeyTreeUtil::isPrime(childNum))
        outputString("  * child number:       " + std::to_string(KeyTreeUtil::removePrime(childNum))+"'");
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

void KeyTreeEditDialog::outputString(const std::string& str) {
    QString qtext = GUIUtil::qStringFromSTDString(str+"");
    ui->resultsTextEdit->append(qtext);
}

void KeyTreeEditDialog::clearTree()
{
    ui->resultsTextEdit->clear();
    treeGraphWidget->removeAllItem();
    delete treeGraphWidget;
    treeGraphWidget = new GraphWidget;
    ui->formLayout->addWidget(treeGraphWidget);
}



