//
// Created by root on 9/5/17.
//

#ifndef PROJECT_PACKUNPACK_H
#define PROJECT_PACKUNPACK_H

#include <msgpack.hpp>


template<class T>
inline void ReadMapValue(msgpack::object& obj, std::string key, T& t)
{
    t = T();
}

template<>
inline void ReadMapValue(msgpack::object& obj, std::string key, std::string& value)
{
    value = "";
    msgpack::object_kv*  pkv;
    msgpack::object_kv*  pkv_end;
    msgpack::object      pk, pv;
    if (obj.type == msgpack::v1::type::MAP)
    {
        pkv = obj.via.map.ptr;
        pkv_end = obj.via.map.ptr + obj.via.map.size;
        do
        {
            pk = pkv->key;
            pv = pkv->val;
            if (pk.as<std::string>() == key )
            {
                if(pv.type == msgpack::v1::type::STR)
                    pv.convert(value);
                break;
            }
            ++pkv;
        } while (pkv < pkv_end);
    }
}

template<>
inline void ReadMapValue(msgpack::object& obj, std::string key, int& value)
{
    value = 0;
    msgpack::object_kv*  pkv;
    msgpack::object_kv*  pkv_end;
    msgpack::object      pk, pv;
    if (obj.type ==  msgpack::v1::type::MAP)
    {
        pkv = obj.via.map.ptr;
        pkv_end = obj.via.map.ptr + obj.via.map.size;
        do
        {
            pk = pkv->key;
            pv = pkv->val;
            if (pk.as<std::string>() == key )
            {
                if(pv.type == msgpack::v1::type::POSITIVE_INTEGER)
                    pv.convert(value);
                break;
            }
            ++pkv;
        } while (pkv < pkv_end);
    }
}

template<>
inline void ReadMapValue(msgpack::object& obj, std::string key, short& value)
{
    value = 0;
    msgpack::object_kv*  pkv;
    msgpack::object_kv*  pkv_end;
    msgpack::object      pk, pv;
    if (obj.type ==  msgpack::v1::type::MAP)
    {
        pkv = obj.via.map.ptr;
        pkv_end = obj.via.map.ptr + obj.via.map.size;
        do
        {
            pk = pkv->key;
            pv = pkv->val;
            if (pk.as<std::string>() == key )
            {
                if(pv.type == msgpack::v1::type::POSITIVE_INTEGER)
                    pv.convert(value);
                break;
            }
            ++pkv;
        } while (pkv < pkv_end);
    }
}

template<>
inline void ReadMapValue(msgpack::object& obj, std::string key, char& value)
{
    value = 0;
    msgpack::object_kv*  pkv;
    msgpack::object_kv*  pkv_end;
    msgpack::object      pk, pv;
    if (obj.type ==  msgpack::v1::type::MAP)
    {
        pkv = obj.via.map.ptr;
        pkv_end = obj.via.map.ptr + obj.via.map.size;
        do
        {
            pk = pkv->key;
            pv = pkv->val;
            if (pk.as<std::string>() == key )
            {
                if(pv.type == msgpack::v1::type::POSITIVE_INTEGER)
                    pv.convert(value);
                break;
            }
            ++pkv;
        } while (pkv < pkv_end);
    }
}

//从map里面读取一个对象，目前只支持读取map array
inline void ReadMapValue(msgpack::object& obj, std::string key, msgpack::object& value)
{
    value = msgpack::object();
    msgpack::object_kv*  pkv;
    msgpack::object_kv*  pkv_end;
    msgpack::object      pk, pv;
    if (obj.type ==  msgpack::v1::type::MAP)
    {
        pkv = obj.via.map.ptr;
        pkv_end = obj.via.map.ptr + obj.via.map.size;
        do
        {
            pk = pkv->key;
            pv = pkv->val;
            if (pk.as<std::string>() == key )
            {
                if(pv.type == msgpack::v1::type::ARRAY || pv.type == msgpack::v1::type::MAP)
                    value = pv;
                break;
            }
            ++pkv;
        } while (pkv < pkv_end);
    }
}

template<class T>
inline void ReadArrayValue(msgpack::object& array, std::size_t index, std::string key, T& value)
{
    value = T();
}

template<>
inline void ReadArrayValue(msgpack::object& array, std::size_t index, std::string key, int& value)
{
    value = 0;
    msgpack::object*     pav;
    msgpack::object_kv*	 pkv;
    msgpack::object_kv*  pkv_end;
    msgpack::object      pk, pv;

    if (array.type == msgpack::v1::type::ARRAY)
    {
        if (index < array.via.array.size)
        {
            pav = array.via.array.ptr + index;
            if (pav->type == msgpack::v1::type::MAP)
            {
                pkv = pav->via.map.ptr;
                pkv_end = pav->via.map.ptr + pav->via.map.size;
                do
                {
                    pk = pkv->key;
                    pv = pkv->val;
                    if (pk.as<std::string>() == key )
                    {
                        if(pv.type == msgpack::v1::type::POSITIVE_INTEGER)
                            pv.convert(value);
                        break;
                    }
                    ++pkv;
                } while (pkv < pkv_end);
            }
        }
    }
}

template<>
inline void ReadArrayValue(msgpack::object& array, std::size_t index, std::string key, short& value)
{
    value = 0;
    msgpack::object*     pav;
    msgpack::object_kv*	 pkv;
    msgpack::object_kv*  pkv_end;
    msgpack::object      pk, pv;

    if (array.type == msgpack::v1::type::ARRAY)
    {
        if (index < array.via.array.size)
        {
            pav = array.via.array.ptr + index;
            if (pav->type ==  msgpack::v1::type::MAP)
            {
                pkv = pav->via.map.ptr;
                pkv_end = pav->via.map.ptr + array.via.map.size;
                do
                {
                    pk = pkv->key;
                    pv = pkv->val;
                    if (pk.as<std::string>() == key )
                    {
                        if(pv.type == msgpack::v1::type::POSITIVE_INTEGER)
                            pv.convert(value);
                        break;
                    }
                    ++pkv;
                } while (pkv < pkv_end);
            }
        }
    }
}


template<>
inline void ReadArrayValue(msgpack::object& array, std::size_t index,std::string key, std::string& value)
{
    value = "";
    msgpack::object*     pav;
    msgpack::object_kv*	 pkv;
    msgpack::object_kv*  pkv_end;
    msgpack::object      pk, pv;

    if (array.type == msgpack::v1::type::ARRAY)
    {
        if (index < array.via.array.size)
        {
            pav = array.via.array.ptr + index;
            if (pav->type ==  msgpack::v1::type::MAP)
            {
                pkv = pav->via.map.ptr;
                pkv_end = pav->via.map.ptr + array.via.map.size;
                do
                {
                    pk = pkv->key;
                    pv = pkv->val;
                    if (pk.as<std::string>() == key )
                    {
                        if (pv.type == msgpack::v1::type::STR)
                            pv.convert(value);
                        break;
                    }
                    ++pkv;
                } while (pkv < pkv_end);
            }
        }
    }
}


template<>
inline void ReadArrayValue(msgpack::object& array, std::size_t index, std::string key, char& value)
{
    value = 0;
    msgpack::object*     pav;
    msgpack::object_kv*	 pkv;
    msgpack::object_kv*  pkv_end;
    msgpack::object      pk, pv;

    if (array.type == msgpack::v1::type::ARRAY)
    {
        if (index < array.via.array.size)
        {
            pav = array.via.array.ptr + index;
            if (pav->type ==  msgpack::v1::type::MAP)
            {
                pkv = pav->via.map.ptr;
                pkv_end = pav->via.map.ptr + array.via.map.size;
                do
                {
                    pk = pkv->key;
                    pv = pkv->val;
                    if (pk.as<std::string>() == key )
                    {
                        if(pv.type == msgpack::v1::type::POSITIVE_INTEGER)
                            pv.convert(value);
                        break;
                    }
                    ++pkv;
                } while (pkv < pkv_end);
            }
        }
    }
}



#define ReadMapData(obj, key, data)         ReadMapValue(obj,key,data);

#define ReadArrayData(obj, i, key, data)    ReadArrayValue(obj,i,key,data);

#define WriteKeyValue(pack, key, value)     pack.pack(key);pack.pack(value);

#define WriteKey(pack, data)                pack.pack(data);

#define WriteValue(pack, data)              pack.pack(data);

#define WriteMap(pack, size)                pack.pack_map(size);

#define WriteArray(pack, size)              pack.pack_array(size);

/// var 转换成字符串 "var"
#define NAME_TO_STR(var)                    (#var)

#endif //PROJECT_PACKUNPACK_H
