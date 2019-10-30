#ifndef STRINGMATCHER_H
#define STRINGMATCHER_H

#include <QStringList>

class NameMatcher
{
public:
    NameMatcher();
    static QList<QPair<int, int> > match(const QStringList &names1, const QStringList &names2);
};

#endif // STRINGMATCHER_H
