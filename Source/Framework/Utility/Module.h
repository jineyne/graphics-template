#pragma once

#include "pch.h"
#include "Exceptions.h"

namespace gt {
    template <typename T>
    class Module {
    public:
        // 인스턴스를 획득한다
        static T &Instance() {
            if (!IsStartedUp()) {
                throw invalid_state("Trying to access a module but it hasn't been started up yet.");
            }

            if (IsDestroyed()) {
                throw invalid_state("Trying to access a destroyed module.");
            }

            return *InstanceInternal();
        }

        // 인스턴스 포인터를 획득한다
        static T *InstancePtr() {
            if (!IsStartedUp()) {
                throw invalid_state("Trying to access a module but it hasn't been started up yet.");
            }

            if (IsDestroyed()) {
                throw invalid_state("Trying to access a destroyed module.");
            }

            return InstanceInternal();
        }

        // 모듈을 시작한다
        template <class ...Args>
        static void StartUp(Args && ...args) {
            if (IsStartedUp()) {
                throw invalid_state("Trying to start an already started module.");
            }

            if (IsDestroyed()) {
                throw invalid_state("Trying to start an already shut down module.");
            }

            // 
            InstanceInternal() = new T(std::forward<Args>(args)...);
            static_cast<Module *>(InstanceInternal())->onStartUp();

            IsStartedUp() = true;
        }

        // 상속된 클래스로 모듈을 시작한다
        template <typename U, class ...Args>
        static void StartUp(Args && ...args) {
            static_assert(std::is_base_of<T, U>::value, "Provided type is not derived from type the Module is initialized with.");

            if (IsStartedUp()) {
                throw invalid_state("Trying to start an already started module.");
            }

            if (IsDestroyed()) {
                throw invalid_state("Trying to start an already shut down module.");
            }

            // 
            InstanceInternal() = new U(std::forward<Args>(args)...);
            static_cast<Module *>(InstanceInternal())->onStartUp();

            IsStartedUp() = true;
        }

        // 모듈을 종료한다
        template <class ...Args>
        static void ShutDown(Args &&...args) {
            if (!IsStartedUp()) {
                throw invalid_state("Trying to shut down a module which was never started.");
            }

            if (IsDestroyed()) {
                throw invalid_state("Trying to shut down an already shut down module.");
            }

            static_cast<Module *>(InstanceInternal())->onShutDown();

            delete InstanceInternal();

            InstanceInternal() = nullptr;
            IsDestroyed() = true;
        }

        static bool IsRunning() {
            return IsStartedUp() && !IsDestroyed();
        }

    protected:
        // 해당 모듈이 시작될 때 호출됨 
        virtual void onStartUp() {}

        // 해당 모듈이 종료될 때 호출됨
        virtual void onShutDown() {}

    private:
        static T *&InstanceInternal() {
            static T *inst = nullptr;
            return inst;
        }

        static bool &IsStartedUp() {
            static bool inst = false;
            return inst;
        }

        static bool &IsDestroyed() {
            static bool inst = false;
            return inst;
        }
    };
}
