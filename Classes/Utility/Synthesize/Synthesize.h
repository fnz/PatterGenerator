#pragma once

#define MapIntInt std::map<int, int>

// Factory with autorelease
#define CREATE_WITH_AUTORELEASE(__TYPE__) \
    static __TYPE__* create() {           \
        __TYPE__* pRet = new __TYPE__();  \
        if (pRet && pRet->init()) {       \
            pRet->autorelease();          \
            return pRet;                  \
        } else {                          \
            delete pRet;                  \
            pRet = NULL;                  \
            return NULL;                  \
        }                                 \
    }

// Synthesize
#define SYNTHESIZE(varType, varName, funName)  \
protected:                                     \
    varType varName;                           \
                                               \
public:                                        \
    virtual varType get##funName(void) const { \
        return varName;                        \
    }                                          \
                                               \
public:                                        \
    virtual void set##funName(varType var) {   \
        varName = var;                         \
    }

#define SYNTHESIZE_READONLY(varType, varName, funName) \
protected:                                             \
    varType varName;                                   \
                                                       \
public:                                                \
    virtual varType get##funName(void) const {         \
        return varName;                                \
    }

#define SYNTHESIZE_READONLY_PASS_BY_REF(varType, varName, funName) \
protected:                                                         \
    varType varName;                                               \
                                                                   \
public:                                                            \
    virtual const varType& get##funName(void) const {              \
        return varName;                                            \
    }
