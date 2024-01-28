#pragma once

#include "sound_bakery/node/node.h"

namespace SB::Engine
{
    class Container;

    /**
     * @brief Contains all information required for gathering sounds for runtime playing and selection.
     */
    struct GatherSoundsContext
    {
        GatherSoundsContext()
        {
            // We're making the rough assumption that each call to gatherSounds
            // will gather, on average, 3 or less sounds.
            // This should hopefully save some allocation time in most cases.
            sounds.reserve(3);
        }

        /**
         * @brief Vector of containers that should play this iteration.
         */
        std::vector<Container*> sounds;

        /**
         * @brief List of parameters that are local to this gathering.
         *
         * Use this to choose sounds based on the current/local context.
         */
        LocalParameterList parameters;
    };

    /**
     * @brief Base container type. Inherited types include sounds, random,
     * sequence etc.
     */
    class Container : public Node
    {
    public:
        Container() : Node() {}

        virtual void onLoaded() override;

        /**
         * @brief Collects and gathers sounds on this node and its children for play.
         *
         * Given a context object with parameters relevant to the local reference, chooses which sounds/containers
         * to play.
         *
         * @param context for this gather sounds call.
         */
        virtual void gatherSounds(GatherSoundsContext& context) {}

        RTTR_ENABLE(Node)
        RTTR_REGISTRATION_FRIEND
    };
}  // namespace SB::Engine