#ifndef KMP_H
#define KMP_H

#include <QString>

class KMP {
public:
    KMP();
    
    static bool contains(const QString &text, const QString &pattern);

private:
    static QList<int> buildLPS(const QString &pattern);
};

#endif // KMP_H