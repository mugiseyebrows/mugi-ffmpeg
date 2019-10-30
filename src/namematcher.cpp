#include "namematcher.h"

#include <QList>

namespace {

class match {
public:
    int pos1;
    int pos2;
    int len;
    match() : pos1(-1), pos2(-1), len(-1) {

    }
    match(int pos1, int pos2, int len) : pos1(pos1), pos2(pos2), len(len) {

    }
};

bool operator >(const match& m1, const match& m2) {
    return m1.len > m2.len;
}

int compare(const QString& name1, const QString& name2) {
    QString name1_ = name1;
    QString name2_ = name2;

    int score = 0;
    while (true) {
        match best;
        for(int i=0;i<name1_.size();i++) {
            for(int j=1;j<=name1_.size() - i;j++) {
                QString sub = name1_.mid(i,j);
                int p = name2_.indexOf(sub);
                //qDebug() << "sub" << sub << "p" << p;
                if (p > -1) {
                    match current(i, p, j);
                    if (current > best) {
                        best = current;
                    }
                } else {
                    break;
                }
            }
        }
        if (best.len < 1) {
            break;
        }
        score += best.len;

        //qDebug() << "best" << name1_.mid(best.pos1, best.len);

        name1_ = name1_.mid(0,best.pos1) + name1_.mid(best.pos1 + best.len);

        name2_ = name2_.mid(0,best.pos2) + name2_.mid(best.pos2 + best.len);

        //qDebug() << "reduced" << name1_ << name2_;
    }

    return score;
}

QList<QPair<int,int> > invertPairs(const QList<QPair<int,int> >& pairs) {
    QList<QPair<int,int> > result;
    for(int i=0;i<pairs.size();i++) {
        result << QPair<int,int>(pairs[i].second,pairs[i].first);
    }
    return result;
}

class score {
public:
    int i;
    int j;
    int points;
    score() : i(-1), j(-1), points(-1) {

    }
    score(int i, int j, int points) : i(i), j(j), points(points) {

    }
};

score bestScore(const QList<QList<int> >& scores,
                         const QSet<int>& used1,
                         const QSet<int>& used2) {
    score best;
    for(int i=0;i<scores.size();i++) {
        if (used1.contains(i)) {
            continue;
        }
        const QList<int>& scores_ = scores[i];
        for(int j=0;j<scores_.size();j++) {
            if (used2.contains(j)) {
                continue;
            }
            if (scores_[j] > best.points) {
                best = score(i,j,scores_[j]);
            }
        }
    }
    return best;
}

}

#if 0
class CacheItem {
    QStringList names1;
    QStringList names2;
    QList<QPair<int,int> > result;
    CacheItem() {

    }
    CacheItem(const QStringList& names1, const QStringList& names2, const QList<QPair<int,int> >& result) :
    names1(names1), names2(names2), result(result){

    }
};
#endif


QList<QPair<int,int> > NameMatcher::match(const QStringList& names1, const QStringList& names2) {

    if (names1.size() > names2.size()) {
        return invertPairs(match(names2, names1));
    }

    QList<QList<int> > scores;
    for(int i=0;i<names1.size();i++) {
        QList<int> scores_;
        for(int j=0;j<names2.size();j++) {
            scores_ << compare(names1[i],names2[j]);
        }
        scores.append(scores_);
    }
    QSet<int> used1;
    QSet<int> used2;

    QList<QPair<int,int> > result;
    for(int i=0;i<names1.size();i++) {
        score best = bestScore(scores, used1, used2);

        //qDebug() << "best" << best.points;

        result << QPair<int,int>(best.i, best.j);
        used1 << best.i;
        used2 << best.j;
    }

    qSort(result.begin(), result.end(), [](const QPair<int,int>& v1, const QPair<int,int>& v2){
        return v1.first < v2.first;
    });

    return result;
}


