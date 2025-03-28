#pragma once

namespace boids
{
    /**
     * \brief Holds all the data that controls the boids behavior.
     */
    struct SimulationData
    {
        /// \brief `true` if the boids should follow the mouse cursor, `false` otherwise.
        bool followMouse = true;

        /// \brief The weight of the separation behavior.
        float separationWeight = 1.5f;

        /// \brief The weight of the alignment behavior.
        float alignmentWeight = 1.f;

        /// \brief The weight of the cohesion behavior.
        float cohesionWeight = 1.f;

        /// \brief The maximum speed of the boids.
        float maxSpeed = 150.f;

        /// \brief The maximum distance a boid is influenced by another.
        float maxDistance = 50.f;

        /// \brief The field of view of the boids, in degrees to filter which ones affect the current boid.
        float fieldOfView = 100.f;
    };
}
