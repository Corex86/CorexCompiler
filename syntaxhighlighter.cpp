#include "syntaxhighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent) {
    setupRules();
}

void SyntaxHighlighter::setLanguage(const QString &lang) {
    currentLanguage = lang;
    setupRules();
    rehighlight();
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
    for (const HighlightingRule &rule : highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    // Multi-line comments
    setCurrentBlockState(0);
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}

void SyntaxHighlighter::setupRules() {
    highlightingRules.clear();

    // Keywords format
    keywordFormat.setForeground(QColor(86, 156, 214));
    keywordFormat.setFontWeight(QFont::Bold);

    // Class format
    classFormat.setForeground(QColor(78, 201, 176));
    classFormat.setFontWeight(QFont::Bold);

    // Single line comment
    singleLineCommentFormat.setForeground(QColor(106, 153, 85));
    singleLineCommentFormat.setFontItalic(true);

    // String format
    quotationFormat.setForeground(QColor(206, 145, 120));

    // Function format
    functionFormat.setForeground(QColor(220, 220, 170));

    // Number format
    numberFormat.setForeground(QColor(181, 206, 168));

    // Multi-line comment format
    multiLineCommentFormat.setForeground(QColor(106, 153, 85));
    multiLineCommentFormat.setFontItalic(true);

    QStringList keywordPatterns;

    if (currentLanguage == "C++" || currentLanguage == "C") {
        keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                      << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                      << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                      << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                      << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                      << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                      << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                      << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                      << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                      << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bbool\\b"
                      << "\\bif\\b" << "\\belse\\b" << "\\bfor\\b" << "\\bwhile\\b"
                      << "\\bdo\\b" << "\\breturn\\b" << "\\bswitch\\b" << "\\bcase\\b"
                      << "\\bbreak\\b" << "\\bcontinue\\b" << "\\bdefault\\b"
                      << "\\bauto\\b" << "\\bconstexpr\\b" << "\\bdecltype\\b"
                      << "\\binclude\\b" << "\\bdefine\\b" << "\\bifdef\\b"
                      << "\\bifndef\\b" << "\\bendif\\b" << "\\btrue\\b" << "\\bfalse\\b";

        commentStartExpression = QRegularExpression("/\\*");
        commentEndExpression = QRegularExpression("\\*/");

        HighlightingRule rule;
        rule.pattern = QRegularExpression("//[^\n]*");
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);

    } else if (currentLanguage == "Python") {
        keywordPatterns << "\\bFalse\\b" << "\\bNone\\b" << "\\bTrue\\b"
                      << "\\band\\b" << "\\bas\\b" << "\\bassert\\b"
                      << "\\bbreak\\b" << "\\bclass\\b" << "\\bcontinue\\b"
                      << "\\bdef\\b" << "\\bdel\\b" << "\\belif\\b"
                      << "\\belse\\b" << "\\bexcept\\b" << "\\bfinally\\b"
                      << "\\bfor\\b" << "\\bfrom\\b" << "\\bglobal\\b"
                      << "\\bif\\b" << "\\bimport\\b" << "\\bin\\b"
                      << "\\bis\\b" << "\\blambda\\b" << "\\bnonlocal\\b"
                      << "\\bnot\\b" << "\\bor\\b" << "\\bpass\\b"
                      << "\\braise\\b" << "\\breturn\\b" << "\\btry\\b"
                      << "\\bwhile\\b" << "\\bwith\\b" << "\\byield\\b"
                      << "\\bself\\b" << "\\bprint\\b" << "\\brange\\b";

        HighlightingRule rule;
        rule.pattern = QRegularExpression("#[^\n]*");
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);

        commentStartExpression = QRegularExpression("'''");
        commentEndExpression = QRegularExpression("'''");

    } else if (currentLanguage == "JavaScript") {
        keywordPatterns << "\\bvar\\b" << "\\blet\\b" << "\\bconst\\b"
                      << "\\bfunction\\b" << "\\breturn\\b" << "\\bif\\b"
                      << "\\belse\\b" << "\\bfor\\b" << "\\bwhile\\b"
                      << "\\bdo\\b" << "\\bswitch\\b" << "\\bcase\\b"
                      << "\\bbreak\\b" << "\\bcontinue\\b" << "\\btry\\b"
                      << "\\bcatch\\b" << "\\bfinally\\b" << "\\bthrow\\b"
                      << "\\bnew\\b" << "\\bthis\\b" << "\\bclass\\b"
                      << "\\bextends\\b" << "\\basync\\b" << "\\bawait\\b"
                      << "\\btrue\\b" << "\\bfalse\\b" << "\\bnull\\b"
                      << "\\bundefined\\b" << "\\bconsole\\b";

        commentStartExpression = QRegularExpression("/\\*");
        commentEndExpression = QRegularExpression("\\*/");

        HighlightingRule rule;
        rule.pattern = QRegularExpression("//[^\n]*");
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);
    }

    for (const QString &pattern : keywordPatterns) {
        HighlightingRule rule;
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // Class names
    HighlightingRule rule;
    rule.pattern = QRegularExpression("\\b[A-Z][a-zA-Z0-9_]*\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    // Strings
    rule.pattern = QRegularExpression("\".*\"|'.*'");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    // Functions
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    // Numbers
    rule.pattern = QRegularExpression("\\b[0-9]+\\.?[0-9]*\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);
}