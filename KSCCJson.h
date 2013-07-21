//
//  KSCCJson.h
//  DisasterGuy
//
//  Created by kisupark on 12. 11. 29..
//
//

#ifndef __DisasterGuy__KSCCJson__
#define __DisasterGuy__KSCCJson__

#include <cocos2d.h>
#include <cocos2d/KSCCString.h>
#include <cocos2d/KSCCArray.h>
#include <format/json/json.h>
#include <common/KSCCDefaultMacro.h>

NS_KSCC_BEGIN

class KSCCJson : public cocos2d::CCObject {
protected:
    KSCCJson(){}
public:
    virtual ~KSCCJson();
    
    static cocos2d::CCString* jsonStringFromDictionary(cocos2d::CCDictionary *dic);
    static cocos2d::CCString* jsonStringFromArray(cocos2d::CCArray *arr);
    static Json::Value jsonValueFromArray(cocos2d::CCArray *array);
    static Json::Value jsonValueFromDictionary(cocos2d::CCDictionary *dic);
    static Json::Value jsonValueFromString(cocos2d::CCString *str);
    
    
    static cocos2d::CCObject* objectFromJsonString(const std::string &jsonStr);
    static cocos2d::CCObject* objectFromJsonString(cocos2d::CCString *jsonStr);
    static cocos2d::CCObject* objectFromJsonValue(Json::Value value);
    
    // Check
    static bool isString(cocos2d::CCObject *dest);
    static bool isDictionary(cocos2d::CCObject *dest);
    static bool isArray(cocos2d::CCObject *dest);
};

NS_KSCC_END

#endif /* defined(__DisasterGuy__KSCCJson__) */
