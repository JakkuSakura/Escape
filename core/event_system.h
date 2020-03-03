//
// Created by jack on 20-3-3.
//

#ifndef ESCAPE_EVENT_SYSTEM_H
#define ESCAPE_EVENT_SYSTEM_H

#include "MyECS.h"
#include <entt/signal/dispatcher.hpp>

struct Event : public VirtualBase {

};

struct Collision {
    entt::entity a;
    entt::entity b;
};

namespace Escape {
    class EventSystem : public ECSSystem {
        template<typename ... T>
        struct FunctionWrapper : public VirtualBase {
            std::function<void(T ...)> func;

            FunctionWrapper(std::function<void(T ...)> fn) : func(fn) {}

            void call(T ... args) {
                func(args ...);
            }
        };

        entt::dispatcher dispatcher{};
        std::vector<VirtualBase *> wrappers;
    public:
        EventSystem() {

        }

        ~EventSystem() {
            for (VirtualBase *vb : wrappers) {
                delete vb;
            }
        }

        auto &getDispatcher() {
            return dispatcher;
        }


        void update(float delta) override {
            dispatcher.update();
        }


        template<class Ret, class Arg1, class ...Args>
        constexpr auto curry(Ret f(Arg1, Args...), Arg1 arg)
        -> std::function<Ret(Args...)> {
            return [=](Args ...args) { return f(arg, args...); };
        }

        template<typename Fn>
        constexpr auto bind(Fn fn) {
            return fn;
        }

        template<typename Fn, typename Bingding_1, typename ... Bindings>
        constexpr auto bind(Fn fn, Bingding_1 b1, Bindings ... bindings) {
            return bind(curry(fn, b1), bindings ...);
        }


        template<typename ... Arg>
        constexpr void listen(std::function<void(Arg ...)> fn) {
            auto *wrapper = new FunctionWrapper(fn);
            dispatcher.sink<Arg ...>().template connect<&FunctionWrapper<Arg ...>::call>(*wrapper);
            wrappers.push_back(wrapper);
        }

        template<typename Fn, typename ... Bindings>
        constexpr void listen(Fn fn, Bindings ... bindings) {
            listen(bind(fn, bindings ...));
        }

        template<typename T>
        void enqueue(T event) {
            dispatcher.enqueue(event);
        }

        template<typename T>
        void trigger(T event) {
            dispatcher.trigger(event);
        }

    };
}


#endif //ESCAPE_EVENT_SYSTEM_H
