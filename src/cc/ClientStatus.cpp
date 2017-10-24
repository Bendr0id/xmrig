/* XMRig
 * Copyright 2010      Jeff Garzik <jgarzik@pobox.com>
 * Copyright 2012-2014 pooler      <pooler@litecoinpool.org>
 * Copyright 2014      Lucas Jones <https://github.com/lucasjones>
 * Copyright 2014-2016 Wolf9466    <https://github.com/OhGodAPet>
 * Copyright 2016      Jay D Dee   <jayddee246@gmail.com>
 * Copyright 2016-2017 XMRig       <support@xmrig.com>
 * Copyright 2017-     BenDr0id    <ben@graef.in>
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#include <chrono>
#include <cstring>
#include <3rdparty/rapidjson/stringbuffer.h>
#include <3rdparty/rapidjson/prettywriter.h>
#include <log/Log.h>

#include "cc/ClientStatus.h"

ClientStatus::ClientStatus()
    : m_hashrateShort(0),
      m_hashrateMedium(0),
      m_hashrateLong(0),
      m_sharesGood(0),
      m_sharesTotal(0),
      m_hashesTotal(0),
      m_lastStatusUpdate(0)
{

}

const std::string ClientStatus::getClientId() const
{
    return m_clientId;
}

void ClientStatus::setClientId(const std::string& clientId)
{
    m_clientId = clientId;
}

const std::string ClientStatus::getCurrentPool() const
{
    return m_currentPool;
}

void ClientStatus::setCurrentPool(const std::string& currentPool)
{
    m_currentPool = currentPool;
}

ClientStatus::Status ClientStatus::getCurrentStatus() const
{
    return m_currentStatus;
}

void ClientStatus::setCurrentStatus(Status currentStatus)
{
    m_currentStatus = currentStatus;
}

double ClientStatus::getHashrateShort() const
{
    return m_hashrateShort;
}

void ClientStatus::setHashrateShort(double hashrateShort)
{
    m_hashrateShort = hashrateShort;
}

double ClientStatus::getHashrateMedium() const
{
    return m_hashrateMedium;
}

void ClientStatus::setHashrateMedium(double hashrateMedium)
{
    m_hashrateMedium = hashrateMedium;
}

double ClientStatus::getHashrateLong() const
{
    return m_hashrateLong;
}

void ClientStatus::setHashrateLong(double hashrateLong)
{
    m_hashrateLong = hashrateLong;
}

uint64_t ClientStatus::getSharesGood() const
{
    return m_sharesGood;
}

void ClientStatus::setSharesGood(uint64_t sharesGood)
{
    m_sharesGood = sharesGood;
}

uint64_t ClientStatus::getSharesTotal() const
{
    return m_sharesTotal;
}

void ClientStatus::setSharesTotal(uint64_t sharesTotal)
{
    m_sharesTotal = sharesTotal;
}

uint64_t ClientStatus::getHashesTotal() const
{
    return m_hashesTotal;
}

void ClientStatus::setHashesTotal(uint64_t hashesTotal)
{
    m_hashesTotal = hashesTotal;
}

void ClientStatus::setHashrateHighest(double hashrateHighest)
{
    m_hashrateHighest = hashrateHighest;
}

double ClientStatus::getHashrateHighest() const
{
    return m_hashrateHighest;
}

void ClientStatus::setAvgTime(uint32_t avgTime)
{
    m_avgTime = avgTime;
}

uint32_t ClientStatus::getAvgTime() const
{
    return m_avgTime;
}

std::time_t ClientStatus::getLastStatusUpdate() const
{
    return m_lastStatusUpdate;
}

bool ClientStatus::parseFromJson(const rapidjson::Document& document)
{
    bool result = false;

    if (document.HasMember("client_status"))
    {
        rapidjson::Value::ConstObject clientStatus = document["client_status"].GetObject();

        if (clientStatus.HasMember("client_id")) {
            m_clientId = clientStatus["client_id"].GetString();
        }

        if (clientStatus.HasMember("current_pool")) {
            m_currentPool = clientStatus["current_pool"].GetString();
        }

        if (clientStatus.HasMember("current_status")) {
            m_currentStatus = toStatus(clientStatus["current_status"].GetString());
        }

        if (clientStatus.HasMember("hashrate_short")) {
            m_hashrateShort = clientStatus["hashrate_short"].GetDouble();
        }

        if (clientStatus.HasMember("hashrate_medium")) {
            m_hashrateMedium = clientStatus["hashrate_medium"].GetDouble();
        }

        if (clientStatus.HasMember("hashrate_long")) {
            m_hashrateLong = clientStatus["hashrate_long"].GetDouble();
        }

        if (clientStatus.HasMember("hashrate_highest")) {
            m_hashrateHighest = clientStatus["hashrate_highest"].GetDouble();
        }

        if (clientStatus.HasMember("avg_time")) {
            m_avgTime = clientStatus["avg_time"].GetUint();
        }

        if (clientStatus.HasMember("shares_good")) {
            m_sharesGood = clientStatus["shares_good"].GetUint64();
        }

        if (clientStatus.HasMember("shares_total")) {
            m_sharesTotal = clientStatus["shares_total"].GetUint64();
        }

        if (clientStatus.HasMember("hashes_total")) {
            m_hashesTotal = clientStatus["hashes_total"].GetUint64();
        }

        auto time_point = std::chrono::system_clock::now();
        m_lastStatusUpdate = std::chrono::system_clock::to_time_t(time_point);

        result = true;
    } else {
        LOG_ERR("Parse Error, JSON does not contain: control_command");
    }

    return result;
}

rapidjson::Value ClientStatus::toJson(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator)
{
    rapidjson::Value clientStatus(rapidjson::kObjectType);

    clientStatus.AddMember("client_id", rapidjson::StringRef(m_clientId.c_str()), allocator);
    clientStatus.AddMember("current_pool", rapidjson::StringRef(m_currentPool.c_str()), allocator);
    clientStatus.AddMember("current_status", rapidjson::StringRef(toString(m_currentStatus)), allocator);

    clientStatus.AddMember("hashrate_short", m_hashrateShort, allocator);
    clientStatus.AddMember("hashrate_medium", m_hashrateMedium, allocator);
    clientStatus.AddMember("hashrate_long", m_hashrateLong, allocator);
    clientStatus.AddMember("hashrate_highest", m_hashrateHighest, allocator);
    clientStatus.AddMember("avg_time", m_avgTime, allocator);

    clientStatus.AddMember("shares_good", m_sharesGood, allocator);
    clientStatus.AddMember("shares_total", m_sharesTotal, allocator);
    clientStatus.AddMember("hashes_total", m_hashesTotal, allocator);

    clientStatus.AddMember("last_status_update", m_lastStatusUpdate, allocator);

    return clientStatus;
}

std::string ClientStatus::toJsonString()
{
    rapidjson::Document respDocument;
    respDocument.SetObject();

    auto& allocator = respDocument.GetAllocator();

    rapidjson::Value clientStatus = ClientStatus::toJson(allocator);
    respDocument.AddMember("client_status", clientStatus, allocator);

    rapidjson::StringBuffer buffer(0, 4096);
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    writer.SetMaxDecimalPlaces(10);
    respDocument.Accept(writer);

    return strdup(buffer.GetString());
}


