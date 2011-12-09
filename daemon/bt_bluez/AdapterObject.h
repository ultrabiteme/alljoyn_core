/**
 * @file
 * AdapterObject managed object class definition.  BT HCI device access class.
 */

/******************************************************************************
 * Copyright 2009-2011, Qualcomm Innovation Center, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 ******************************************************************************/
#ifndef _ALLJOYN_ADAPTEROBJECT_H
#define _ALLJOYN_ADAPTEROBJECT_H

#include <qcc/platform.h>

#include <vector>

#include <qcc/ManagedObj.h>
#include <qcc/Socket.h>

#include <alljoyn/BusAttachment.h>
#include <alljoyn/ProxyBusObject.h>

#include "BDAddress.h"
#include "BTTransportConsts.h"

#include <Status.h>



namespace ajn {
namespace bluez {

class _AdapterObject : public ProxyBusObject {
  public:

    _AdapterObject() { }
    _AdapterObject(BusAttachment& bus, const qcc::String& path);
    bool operator==(const _AdapterObject& other) const { return address == other.address; }

    QStatus QueryDeviceInfo();

    bool IsEIRCapable() const { return eirCapable; }
    QStatus SetAddress(const qcc::String addrStr) { return address.FromString(addrStr); }
    const BDAddress& GetAddress() const { return address; }
    bool IsDiscovering() const { return discovering; }
    void SetDiscovering(bool disc) { discovering = disc; }
    bool IsPowered() const { return powered; }
    void SetPowered(bool p) { powered = p; }

    QStatus ConfigureInquiryScan(uint16_t window, uint16_t interval, bool interlaced, int8_t txPower);
    QStatus ConfigurePeriodicInquiry(uint16_t minPeriod, uint16_t maxPeriod, uint8_t length, uint8_t maxResponses);
    QStatus ConfigureSimplePairingDebugMode(bool enable);
    QStatus ConfigureClassOfDevice(uint32_t cod);
    QStatus RequestBTRole(const BDAddress& addr, bt::BluetoothRole role);
    QStatus IsMaster(const BDAddress& addr, bool& master);
    QStatus RequestEnterSniffMode(const BDAddress& addr,
                                  uint16_t minInterval,
                                  uint16_t maxInterval,
                                  uint16_t attemptTO,
                                  uint16_t sniffTO);
    QStatus RequestExitSniffMode(const BDAddress& addr);

  private:

    qcc::SocketFd OpenHCI();

    QStatus SendHCIRequest(qcc::SocketFd hciFd,
                           uint16_t opcode, uint64_t evtMask,
                           const std::vector<uint8_t>& args,
                           std::vector<uint8_t>& rsp);

    QStatus RecvHCIEvent(qcc::SocketFd hciFd, uint64_t evtMask, uint8_t& event, std::vector<uint8_t>& rsp);

    QStatus SetEventFilter(qcc::SocketFd hciFd, uint16_t opcode, uint64_t evtMask);

    uint16_t id;
    BDAddress address;
    bool eirCapable;
    bool discovering;
    bool powered;
};

typedef qcc::ManagedObj<_AdapterObject> AdapterObject;


} // namespace bluez
} // namespace ajn

#endif
