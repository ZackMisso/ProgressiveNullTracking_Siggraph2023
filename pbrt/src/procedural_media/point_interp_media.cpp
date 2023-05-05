#include "point_interp_media.h"
#include "interaction.h"

namespace pbrt
{

Point_Interp_Media::Point_Interp_Media(const Bounds3f& medBounds)
    : medBounds(medBounds)
{
    densities = std::vector<PointDensity>();
    interp = INTERP_LI; // currently the only supported method
}

// This texture is only applied to medium
Spectrum Point_Interp_Media::Evaluate(const SurfaceInteraction& si) const
{
    return Spectrum(1.0) - Evaluate(si.p);
}

Spectrum Point_Interp_Media::Evaluate(const Point3f& p) const
{
    if (!Inside(p, medBounds))
    {
        return Spectrum(0.f);
    }

    // // INVERSE WEIGHTING
    // Float totalWeight = 0.0;
    // Spectrum density = Spectrum(0.0);
    //
    // for (int i = 0; i < densities.size(); ++i)
    // {
    //     Float dist = (p - densities[i].location).Length();
    //     Float weight = 0.0;
    //     if (dist == 0.0)
    //     {
    //         weight = 10000.0;
    //     }
    //     else
    //     {
    //         weight = std::min(10000.0, exp(-dist/(10.0*1.0)));
    //     }
    //     // std::cout << "Weight: " << weight << std::endl;
    //
    //     totalWeight += weight;
    //     density += densities[i].density * weight;
    // }
    //
    // return density / totalWeight;



    switch (interp)
    {
        case INTERP_NN:
        {
            // TODO
        }
        case INTERP_LI:
        {
            int indexOne = 0;
            int indexTwo = densities.size() - 1;
            Float distOne = (p - densities[0].location).Length();
            Float distTwo = (p - densities[densities.size() - 1].location).Length();

            if (distOne > distTwo)
            {
                Float tmp = distTwo;
                int tmpInd = indexTwo;
                indexTwo = indexOne;
                distTwo = distOne;
                indexOne = tmpInd;
                distOne = tmp;
            }

            for (int i = 1; i < densities.size() - 1; ++i)
            {
                Float tmpDist = (p - densities[i].location).Length();
                int tmpIndex = i;
                if (tmpDist < distOne)
                {
                    Float tmp = distOne;
                    distOne = tmpDist;
                    tmpDist = tmp;
                    tmpIndex = indexOne;
                    indexOne = i;
                }
                if (tmpDist < distTwo)
                {
                    distTwo = tmpDist;
                    indexTwo = tmpIndex;
                }
            }

            // std::cout << "A: " << densities[indexOne].location << std::endl;
            // std::cout << "B: " << densities[indexTwo].location << std::endl;
            // std::cout << "C: " << p << std::endl;

            Float distAB = (densities[indexOne].location - densities[indexTwo].location).Length();
            distAB *= distOne;

            Float t = 0.5;

            if (distAB != 0.0)
            {
                t = Dot((densities[indexTwo].location - densities[indexOne].location), (p-densities[indexOne].location)) / distAB;
            }

            // std::cout << "t: " << t << std::endl;
            // Float t = distOne * / (distOne + distTwo);
            // std::cout << t << std::endl;
            // std::cout << p[0] << " " << p[1] << " " << p[2] << std::endl;

            return densities[indexOne].density * (t) + densities[indexTwo].density * (1.0-t);
        }
        case INTERP_BILI:
        {
            // TODO
        }
        case INTERP_BARY:
        {
            // TODO
        }
    }

    return Spectrum(0.f);
}

Point_Interp_Media* CreatePointInterpMedia(const TextureParams& tp)
{
    ParamSet params = tp.GetMaterialParams();

    Bounds3f medBounds = Bounds3f(params.FindOnePoint3f("minBounds", Point3f(0.0, 0.0, 0.0)),
                                  params.FindOnePoint3f("maxBounds", Point3f(1.0, 1.0, 1.0)));

    Point_Interp_Media* media = new Point_Interp_Media(medBounds);

    int numDen = 0;
    const Spectrum* colors = params.FindSpectrum("densities", &numDen);

    int numLoc = 0;
    const Point3f* locs = params.FindPoint3f("locations", &numLoc);

    if (numLoc != numDen)
    {
        std::cout << "ERROR: Number of densities and locations should be the same" << std::endl;
        return nullptr;
    }

    media->densities.resize(numDen);
    std::cout << "NUMDEN: " << numDen << std::endl;
    for (int i = 0; i < numDen; ++i)
    {
        media->densities[i].density = colors[i];
        media->densities[i].location = locs[i];
        media->densities[i].location[0] = media->densities[i].location[0] * (medBounds.pMax.x - medBounds.pMin.x) + medBounds.pMin.x;
        media->densities[i].location[1] = media->densities[i].location[1] * (medBounds.pMax.y - medBounds.pMin.y) + medBounds.pMin.y;
        media->densities[i].location[2] = media->densities[i].location[2] * (medBounds.pMax.z - medBounds.pMin.z) + medBounds.pMin.z;
    }

    return media;
}

}
