//
// Created by jzy99 on 2021/1/1.
//

#pragma once

#include<phoenix/core/phoenix.h>
#include<phoenix/core/color.h>

PHOENIX_NAMESPACE_BEGIN

    class PropertyList {
    public:
        PropertyList() { }

        /// Set a boolean property
        void SetBoolean(const std::string &name, const bool &value);

        /// Get a boolean property, and throw an exception if it does not exist
        bool GetBoolean(const std::string &name) const;

        /// Get a boolean property, and use a default value if it does not exist
        bool GetBoolean(const std::string &name, const bool &defaultValue) const;

        /// Set an integer property
        void SetInteger(const std::string &name, const int &value);

        /// Get an integer property, and throw an exception if it does not exist
        int GetInteger(const std::string &name) const;

        /// Get am integer property, and use a default value if it does not exist
        int GetInteger(const std::string &name, const int &defaultValue) const;

        /// Set a float property
        void SetFloat(const std::string &name, const float &value);

        /// Get a float property, and throw an exception if it does not exist
        float GetFloat(const std::string &name) const;

        /// Get a float property, and use a default value if it does not exist
        float GetFloat(const std::string &name, const float &defaultValue) const;

        /// Set a string property
        void SetString(const std::string &name, const std::string &value);

        /// Get a string property, and throw an exception if it does not exist
        std::string GetString(const std::string &name) const;

        /// Get a string property, and use a default value if it does not exist
        std::string GetString(const std::string &name, const std::string &defaultValue) const;

        /// Set a color property
        void SetColor(const std::string &name, const Color3f &value);

        /// Get a color property, and throw an exception if it does not exist
        Color3f GetColor(const std::string &name) const;

        /// Get a color property, and use a default value if it does not exist
        Color3f GetColor(const std::string &name, const Color3f &defaultValue) const;

        /// Set a point property
        void SetPoint(const std::string &name, const Point3f &value);

        /// Get a point property, and throw an exception if it does not exist
        Point3f GetPoint(const std::string &name) const;

        /// Get a point property, and use a default value if it does not exist
        Point3f GetPoint(const std::string &name, const Point3f &defaultValue) const;

        /// Set a vector property
        void SetVector(const std::string &name, const Vector3f &value);

        /// Get a vector property, and throw an exception if it does not exist
        Vector3f GetVector(const std::string &name) const;

        /// Get a vector property, and use a default value if it does not exist
        Vector3f GetVector(const std::string &name, const Vector3f &defaultValue) const;

        /// Set a transform property
        void SetTransform(const std::string &name, const Transform &value);

        /// Get a transform property, and throw an exception if it does not exist
        Transform GetTransform(const std::string &name) const;

        /// Get a transform property, and use a default value if it does not exist
        Transform GetTransform(const std::string &name, const Transform &defaultValue) const;
    private:
        /* Custom variant data type (stores one of boolean/integer/float/...) */
        struct Property {
            enum {
                boolean_type, integer_type, float_type,
                string_type, color_type, point_type,
                vector_type, transform_type
            } type;

            struct Value
            {
                Value() : boolean_value(false) { }
                ~Value() { }
                bool boolean_value;
                int integer_value;
                float float_value;
                std::string string_value;
                Color3f color_value;
                Point3f point_value;
                Vector3f vector_value;
                Transform transform_value;
            } value;

            Property() : type(boolean_type) { }
        };
        std::map<std::string, Property> properties_;
    };


PHOENIX_NAMESPACE_END