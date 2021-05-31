
#ifndef SINGLETON_H
#define SINGLETON_H

#define DECLARE_SINGLETON(class_name) \
protected: \
        class_name();\
        static  class_name* m_pInstance; \
public:  \
        static  class_name* Instance();

#define IMPLEMENT_SINGLETON(class_name) \
        class_name* class_name::m_pInstance = 0;\
        class_name* class_name::Instance() \
        { \
                if(m_pInstance == 0) \
                        m_pInstance = new class_name();\
                return m_pInstance;\
        }

#endif // SINGLETON_H
