//
//  KSCCJson.cpp
//  DisasterGuy
//
//  Created by kisupark on 12. 11. 29..
//
//

#include "KSCCJson.h"

USING_NS_CC;

NS_KSCC_BEGIN

KSCCJson::~KSCCJson()
{
    
}


CCString* KSCCJson::jsonStringFromDictionary(CCDictionary *dic)
{
    Json::Value root = KSCCJson::jsonValueFromDictionary(dic);
    Json::StyledWriter writer;
    std::string jsonString = writer.write(root);
    return CCString::create(jsonString);
}

CCString* KSCCJson::jsonStringFromArray(CCArray *arr)
{
    Json::Value root = KSCCJson::jsonValueFromArray(arr);
    Json::StyledWriter writer;
    std::string jsonString = writer.write(root);
    return CCString::create(jsonString);
}

Json::Value KSCCJson::jsonValueFromArray(CCArray *arr)
{
    Json::Value value;
    for (unsigned int i=0; i<arr->count(); i++) {
        CCObject *obj = arr->objectAtIndex(i);
        CCString *stringVal = dynamic_cast<CCString *>(obj);
        CCArray *arrayVal = dynamic_cast<CCArray *>(obj);
        CCDictionary *dicVal = dynamic_cast<CCDictionary *>(obj);
        
        if (NULL != stringVal) {        // String
            value.append(KSCCJson::jsonValueFromString(stringVal));
        } else if (NULL != arrayVal) {  // Array
            value.append(KSCCJson::jsonValueFromArray(arrayVal));
        } else if (NULL != dicVal) {    // Object
            value.append(KSCCJson::jsonValueFromDictionary(dicVal));
        }
    }
    return value;
}

Json::Value KSCCJson::jsonValueFromDictionary(CCDictionary *dic)
{
    Json::Value value;
    
    CCDictElement *element = NULL;
    CCDICT_FOREACH(dic, element)
    {
        CCObject *obj = element->getObject();
        CCString *stringVal = dynamic_cast<CCString *>(obj);
        CCArray *arrayVal = dynamic_cast<CCArray *>(obj);
        CCDictionary *dicVal = dynamic_cast<CCDictionary *>(obj);
        
        if (NULL != stringVal) {        // String
            value[element->getStrKey()] = KSCCJson::jsonValueFromString(stringVal);
        } else if (NULL != arrayVal) {  // Array
            value[element->getStrKey()] = KSCCJson::jsonValueFromArray(arrayVal);
        } else if (NULL != dicVal) {    // Object
            value[element->getStrKey()] = KSCCJson::jsonValueFromDictionary(dicVal);
        }
    }
    return value;
}

Json::Value KSCCJson::jsonValueFromString(CCString *str)
{
    Json::Value value;
    value = str->m_sString;
    return value;
}

cocos2d::CCObject* KSCCJson::objectFromJsonString(const std::string &jsonStr)
{
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(jsonStr, root);
    if (!parsingSuccessful) {
        return NULL;
    }
    
    return KSCCJson::objectFromJsonValue(root);
}

cocos2d::CCObject* KSCCJson::objectFromJsonString(CCString *jsonStr)
{
    return KSCCJson::objectFromJsonString(jsonStr->m_sString);
}

cocos2d::CCObject* KSCCJson::objectFromJsonValue(Json::Value value)
{
    CCArray *arrayVal = NULL;
    CCDictionary *dicVal = NULL;
    
    if(value.isArray()) {
        arrayVal = new CCArray();
        arrayVal->init();
        arrayVal->autorelease();
    } else if (value.isObject()) {
        dicVal = new CCDictionary();
        dicVal->autorelease();
    }

    if (value.isArray()) {
        for (unsigned int i=0; i<value.size(); i++) {
            if (value[i].isObject()) {
                arrayVal->addObject(KSCCJson::objectFromJsonValue(value[i]));
            } else if (value[i].isString()) {
                arrayVal->addObject(CCString::create(value[i].asString()));
            } else if (value[i].isArray()) {
                arrayVal->addObject(KSCCJson::objectFromJsonValue(value[i]));
            } else if (value[i].isInt()) {
                arrayVal->addObject(CCString::createWithFormat("%d", value[i].asInt()));
            } else if (value[i].isDouble()) {
				arrayVal->addObject(CCString::createWithFormat("%lf", value[i].asDouble()));
			}
        }
        return arrayVal;
    } else {
        Json::ValueIterator itr = value.begin();
        while (itr != value.end()) {
            Json::Value val = value.get(itr.memberName(), "");
            if (value.isObject()) {
                if (val.isObject()) {
                    CCObject *subObj = KSCCJson::objectFromJsonValue(val);
                    dicVal->setObject(subObj, itr.memberName());
                } else if (val.isArray()) {
                    CCObject *subObj = KSCCJson::objectFromJsonValue(val);
                    dicVal->setObject(subObj, itr.memberName());
                } else if (val.isString()) {
                    dicVal->setObject(CCString::create(val.asString()), std::string(itr.memberName()));
                } else if (val.isInt()) {
                    dicVal->setObject(CCString::createWithFormat("%d", val.asInt()), std::string(itr.memberName()));
				} else if (val.isDouble()) {
					dicVal->setObject(CCString::createWithFormat("%lf", val.asDouble()), std::string(itr.memberName()));
				}
            }
            itr++;
        }
        
        if (NULL != dicVal) {
            return dicVal;
        }
    }
    return NULL;
}


bool KSCCJson::isString(cocos2d::CCObject *dest)
{
    CCString *str = dynamic_cast<CCString *>(dest);
    return (NULL != str);
}

bool KSCCJson::isDictionary(CCObject *dest)
{
    CCDictionary *dic = dynamic_cast<CCDictionary *>(dest);
    return (NULL != dic);
}

bool KSCCJson::isArray(CCObject *dest)
{
    CCArray *arr = dynamic_cast<CCArray *>(dest);
    return (NULL != arr);
}


NS_KSCC_END