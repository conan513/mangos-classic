/*
 * This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __SQLOPERATIONS_H
#define __SQLOPERATIONS_H

#include "Common.h"
#include "Utilities/Callback.h"

#include <queue>
#include <vector>
#include <mutex>
#include <memory>

/// ---- BASE ---

class Database;
class SqlConnection;
class SqlDelayThread;
class SqlStmtParameters;

class SqlOperation
{
    public:
        virtual void OnRemove() { delete this; }
        virtual bool Execute(SqlConnection* conn) = 0;
        virtual ~SqlOperation() {}
};

/// ---- ASYNC STATEMENTS / TRANSACTIONS ----

class SqlPlainRequest : public SqlOperation
{
    private:
        const char* m_sql;
    public:
        SqlPlainRequest(const char* sql) : m_sql(mangos_strdup(sql)) {}
        ~SqlPlainRequest() { char* tofree = const_cast<char*>(m_sql); delete[] tofree; }
        bool Execute(SqlConnection* conn) override;
};

class SqlTransaction : public SqlOperation
{
    private:
        std::vector<SqlOperation* > m_queue;

    public:
        SqlTransaction() {}
        ~SqlTransaction();

        void DelayExecute(SqlOperation* sql) { m_queue.push_back(sql); }

        bool Execute(SqlConnection* conn) override;
};

class SqlPreparedRequest : public SqlOperation
{
    public:
        SqlPreparedRequest(int nIndex, SqlStmtParameters* arg);
        ~SqlPreparedRequest();

        bool Execute(SqlConnection* conn) override;

    private:
        const int m_nIndex;
        SqlStmtParameters* m_param;
};

/// ---- ASYNC QUERIES ----

class SqlQuery;                                             /// contains a single async query
class QueryResult;                                          /// the result of one
class SqlQueryHolder;                                       /// groups several async quries
class SqlQueryHolderEx;                                     /// points to a holder, added to the delay thread

class SqlResultQueue
{
    private:
        std::mutex m_mutex;
        std::queue<std::unique_ptr<MaNGOS::IQueryCallback>> m_queue;

    public:
        void Update();
        void Add(MaNGOS::IQueryCallback*);
};

class SqlQuery : public SqlOperation
{
    private:
        std::vector<char> m_sql;
        MaNGOS::IQueryCallback* const m_callback;
        SqlResultQueue* const m_queue;

    public:
        SqlQuery(const char* sql, MaNGOS::IQueryCallback* callback, SqlResultQueue* queue)
            : m_sql(strlen(sql) + 1), m_callback(callback), m_queue(queue)
        {
            memcpy(&m_sql[0], sql, m_sql.size());
        }

        bool Execute(SqlConnection* conn) override;
};

class SqlQueryHolder
{
        friend class SqlQueryHolderEx;
    private:
        typedef std::pair<const char*, QueryResult*> SqlResultPair;
        std::vector<SqlResultPair> m_queries;
    public:
        SqlQueryHolder() {}
        virtual ~SqlQueryHolder();
        bool SetQuery(size_t index, const char* sql);
        bool SetPQuery(size_t index, const char* format, ...) ATTR_PRINTF(3, 4);
        void SetSize(size_t size);
        QueryResult* GetResult(size_t index);
        void SetResult(size_t index, QueryResult* result);
        bool Execute(MaNGOS::IQueryCallback* callback, SqlDelayThread* thread, SqlResultQueue* queue);
        bool ManualExecute(Database* db);
};

class SqlQueryHolderEx : public SqlOperation
{
    private:
        SqlQueryHolder* m_holder;
        MaNGOS::IQueryCallback* m_callback;
        SqlResultQueue* m_queue;
    public:
        SqlQueryHolderEx(SqlQueryHolder* holder, MaNGOS::IQueryCallback* callback, SqlResultQueue* queue)
            : m_holder(holder), m_callback(callback), m_queue(queue) {}
        bool Execute(SqlConnection* conn) override;
};
#endif                                                      //__SQLOPERATIONS_H
