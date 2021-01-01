#pragma once

#include<phoenix/core/phoenix.h>



PHOENIX_NAMESPACE_BEGIN

    class PhoenixObject {
    public:
        enum class PClassType {
            PCamera,

        };


        virtual PClassType getClassType() const = 0;

        virtual string toString() const = 0;

    };


    class PhoenixObjectFactory {
    public:
        typedef std::function<shared_ptr<PhoenixObject> (const PropertyList &)> Constructor;

        /**
         * \brief Register an object constructor with the object factory
         *
         * This function is called by the macro \ref NORI_REGISTER_CLASS
         *
         * \param name
         *     An internal name that is associated with this class. This is the
         *     'type' field found in the scene description XML files
         *
         * \param constr
         *     A function pointer to an anonymous function that is
         *     able to call the constructor of the class.
         */
        static void registerClass(const std::string &name, const Constructor &constr);

        /**
         * \brief Construct an instance from the class of the given name
         *
         * \param name
         *     An internal name that is associated with this class. This is the
         *     'type' field found in the scene description XML files
         *
         * \param propList
         *     A list of properties that will be passed to the constructor
         *     of the class.
         */
        static shared_ptr<PhoenixObject> createInstance(const std::string &name,
                                          const PropertyList &propList) {
//            if (!m_constructors || m_constructors->find(name) == m_constructors->end())
//                throw NoriException("A constructor for class \"%s\" could not be found!", name);
            return (*m_constructors)[name](propList);
        }

    private:
        static std::map<std::string, Constructor> *m_constructors;
    };

#define PHOENIX_REGISTER_CLASS(cls, name) \
    shared_ptr<cls> cls ##_create(const PropertyList &list) { \
        return make_shared<cls>(list); \
    } \
    static struct cls ##_{ \
        cls ##_() { \
            PhoenixObjectFactory::registerClass(name, cls ##_create); \
        } \
    } cls ##__PHOENIX_;







PHOENIX_NAMESPACE_END