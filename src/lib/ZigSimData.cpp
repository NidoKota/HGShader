#include "ZigSimData.hpp"

void zigsimdata::Parse(char* data)
{
    //TODO:Regexでも使って外部ライブラリに頼る部分を減らす

    using namespace rapidjson;

    Document doc = Document();
    doc.Parse(data);

    if(doc.HasParseError()) return;

    Value& dev = doc["device"];
    _device.name = dev["name"].GetString();
    _device.displayheight = dev["displayheight"].GetInt();
    _device.uuid = dev["uuid"].GetString();
    _device.os = dev["os"].GetString();
    _device.osversion = dev["osversion"].GetString();
    _device.displaywidth = dev["displaywidth"].GetInt();

    if(doc.HasMember("timestamp")) timestamp = doc["timestamp"].GetString();

    Value& sensor = doc["sensordata"];
    if(sensor.HasMember("quaternion"))
    {
        Value& qua = sensor["quaternion"];
        _sensorData.quaternion = vec4((float)qua["x"].GetDouble(), (float)qua["y"].GetDouble(), (float)qua["z"].GetDouble(), (float)qua["w"].GetDouble());
    }
    if(sensor.HasMember("compass"))
    {
        Value& com = sensor["compass"];
        _sensorData.compass = (float)com["compass"].GetDouble();
    }
    if(sensor.HasMember("touch"))
    {
        Value& tou = sensor["touch"];
        if (tou.Size() > 0)
        {
            Value& touZero = tou[0];
            _sensorData.touch = vec3((float)touZero["x"].GetDouble(), (float)touZero["y"].GetDouble(), 0);
        }
    }
}