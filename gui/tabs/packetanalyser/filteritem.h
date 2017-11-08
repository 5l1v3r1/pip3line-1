#ifndef FILTERITEM_H
#define FILTERITEM_H

#include <QBitArray>
#include <QObject>
#include <QHash>
#include <QByteArray>
#include <QDateTime>
#include <QRegExp>
#include "packet.h"

class FilterItem : public QObject
{
        Q_OBJECT
    public:
        enum FilterType {
            Data = 0,
            TimeStamp = 1,
            Length = 2,
            Direction = 3,
            Text = 4,
            CID = 5,
            INVALID = -1
        };
        static const QRegExp nameRegexp;
        explicit FilterItem();
        virtual ~FilterItem();
        QString getName() const;
        void setName(const QString &nname);
        bool isReverseSelection() const;
        void setReverseSelection(bool reverse);
        virtual bool selectable(Packet *packet);
        virtual bool operator == (FilterItem const &other) const;
        virtual bool isValid();
        virtual QHash<QString, QString> getConfiguration();
        virtual void setConfiguration(const QHash<QString, QString> &conf);
        static FilterItem *instanciateFromConf(const QHash<QString, QString> &conf);
        FilterType getFilterId() const;
    protected:
        QString name;
        bool reverseSelection;
        FilterType filterId;

};

class FilterData : public FilterItem
{
        Q_OBJECT
    public:
        enum SEARCH_TYPE {
            HEXA = 0,
            UTF8 = 1,
            UTF16 = 2
        };
        explicit FilterData();
        virtual ~FilterData();
        QByteArray getSearchValue() const;
        void setSearchValue(const QByteArray &searchValue);
        void setBitMask(QBitArray bitMask);
        int getStartingOffset() const;
        void setStartingOffset(const int &offset);
        FilterData::SEARCH_TYPE getSearchType() const;
        void setSearchType(const FilterData::SEARCH_TYPE &type);
        QString getHexValueWithMask() const;
        bool selectable(Packet *packet);

        QHash<QString, QString> getConfiguration();
        void setConfiguration(const QHash<QString, QString> &conf);
        bool isValid();
    private:
        QByteArray searchValue;
        int startingOffset;
        char *mask;
        FilterData::SEARCH_TYPE searchType;
};

class FilterTimeStamp : public FilterItem
{
        Q_OBJECT
    public:
        explicit FilterTimeStamp();
        virtual ~FilterTimeStamp();
        bool selectable(Packet *packet);
        QHash<QString, QString> getConfiguration();
        void setConfiguration(const QHash<QString, QString> &conf);
        bool isValid();
        QDateTime getAfter() const;
        void setAfter(const QDateTime &value);
        QDateTime getBefore() const;
        void setBefore(const QDateTime &value);

    private:
        QDateTime after;
        QDateTime before;
};

class FilterLength : public FilterItem
{
        Q_OBJECT
    public:
        enum Operators {
            LESS = 0,
            LESSEQUAL = 1,
            GREATER = 2,
            GREATEREQUAL = 3,
            EQUAL = 4,
            DIFFERENT = 5,
            INVALID = 6
        };
        static const QStringList OperatorsString;
        explicit FilterLength();
        virtual ~FilterLength();
        bool selectable(Packet *packet);
        QHash<QString, QString> getConfiguration();
        void setConfiguration(const QHash<QString, QString> &conf);
        bool isValid();
        int getLengthValue() const;
        void setLengthValue(int value);
        FilterLength::Operators getOp() const;
        void setOp(const FilterLength::Operators &value);
    private:
        int lengthValue;
        FilterLength::Operators op;
};

class FilterDirection : public FilterItem
{
        Q_OBJECT
    public:
        explicit FilterDirection();
        virtual ~FilterDirection();
        bool selectable(Packet *packet);
        QHash<QString, QString> getConfiguration();
        void setConfiguration(const QHash<QString, QString> &conf);
        bool isValid();
        Packet::Direction getDirection() const;
        void setDirection(const Packet::Direction &value);
    private:
        Packet::Direction direction;
};

class FilterText : public FilterItem
{
        Q_OBJECT
    public:
        static const QStringList RegexpTypesStr;
        explicit FilterText();
        virtual ~FilterText();
        bool selectable(Packet *packet);
        QHash<QString, QString> getConfiguration();
        void setConfiguration(const QHash<QString, QString> &conf);
        bool isValid();
        QRegExp getRegexp() const;
        void setRegexp(const QRegExp &value);
        QString getTargetColumn() const;
        void setTargetColumn(QString columnName);
    private:
        QRegExp regexp;
        QString targetColumn;
};

class FilterCIDs : public FilterItem
{
        Q_OBJECT
    public:
        static const QString SEP;
        explicit FilterCIDs();
        virtual ~FilterCIDs();
        bool selectable(Packet *packet);
        QHash<QString, QString> getConfiguration();
        void setConfiguration(const QHash<QString, QString> &conf);
        bool isValid();
        QList<int> getCidList() const;
        void setCidList(const QList<int> &value);

    private:
        QList<int> cidList;
};


class FilterItemsList : public QList<QSharedPointer<FilterItem> >
{
    public:
        explicit FilterItemsList();
        FilterItemsList(FilterItemsList const &other);
        FilterItemsList& operator = (FilterItemsList const &other);
        ~FilterItemsList();
        QSharedPointer<FilterItem> getFilter(const QString &name);
        bool containsFilter(const QString &name);

};

#endif // FILTERITEM_H
