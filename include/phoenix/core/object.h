#pragma once

#include<phoenix/core/phoenix.h>



PHOENIX_NAMESPACE_BEGIN

class PhoenixObject {
public:
    enum class PClassType {
        PCamera,
        PEmitter,
        PIntegrator,
        PSampler,
        PScene,
        PShape,
        PFilm,
        PFilter
    };

    virtual void AddChild(shared_ptr<PhoenixObject> child) {}
    virtual void SetParent(shared_ptr<PhoenixObject> parent) {}
    virtual void Active() {}

    [[nodiscard]] virtual PClassType GetClassType() const = 0;

    virtual string ToString() const = 0;

};


class PhoenixObjectFactory {
public:
    typedef std::function<shared_ptr<PhoenixObject>(const PropertyList&)> Constructor;

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
    static void MRegisterClass(const std::string& name, const Constructor& constr);

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
    static shared_ptr<PhoenixObject> CreateInstance(const std::string& name,
        const PropertyList& propList) {
        //            if (!m_constructors || m_constructors->find(name) == m_constructors->end())
        //                throw NoriException("A constructor for class \"%s\" could not be found!", name);
        return (*constructors_)[name](propList);
    }

private:
    static std::map<std::string, Constructor>* constructors_;
};

#define PHOENIX_REGISTER_CLASS(cls, name) \
    shared_ptr<cls> cls ##_create(const PropertyList &list) { \
        return make_shared<cls>(list); \
    } \
    static struct cls ##_{ \
        cls ##_() { \
            PhoenixObjectFactory::MRegisterClass(name, cls ##_create); \
        } \
    } cls ##__PHOENIX_;\







PHOENIX_NAMESPACE_END