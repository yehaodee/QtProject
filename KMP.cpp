#include "KMP.h"
#include <QList>

KMP::KMP() {}

QList<int> KMP::buildLPS(const QString &pattern) {
    int m = pattern.length();
    QList<int> lps;
    for (int i = 0; i < m; ++i) {
        lps.append(0);
    }
    
    int len = 0;
    for (int i = 1; i < m; ) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    
    return lps;
}

bool KMP::contains(const QString &text, const QString &pattern) {
    if (pattern.isEmpty()) return true;
    if (text.isEmpty()) return false;
    
    int n = text.length();
    int m = pattern.length();
    
    QList<int> lps = buildLPS(pattern);
    
    int i = 0;
    int j = 0;
    
    while (i < n) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }
        
        if (j == m) {
            return true;
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    
    return false;
}