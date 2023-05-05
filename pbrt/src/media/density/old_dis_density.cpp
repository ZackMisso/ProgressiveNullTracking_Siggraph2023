// THIS IS KEPT IN CASE I NEED THE ORIGINAL COMMENTED OUT CODE

// #include "../../ImageEditLib/include/imedit/noise.h"
// #include "proc_sphere_density.h"

// namespace pbrt {

// ProceduralSphereDensity::ProceduralSphereDensity(Spectrum dense)
//     : dense(dense) {}

// ProceduralSphereDensity::ProceduralSphereDensity(Spectrum dense, Float xpos,
//                                                  Float ypos, Float zpos,
//                                                  Float falloff,
//                                                  Float cutoff_dense)
//     : dense(dense),
//       xpos(xpos),
//       ypos(ypos),
//       zpos(zpos),
//       falloff(falloff),
//       cutoff_dense(cutoff_dense) {}

// Spectrum ProceduralSphereDensity::proc_D(const Point3f& p,
//                                          bool buildState) const {
//     // stage 2 //

//     // { return dense; }

//     // stage 3 //

//     // {
//     //     Float sphere_factor =
//     //         std::sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
//     //     if (sphere_factor < 0.55)
//     //         sphere_factor = 1.0;
//     //     else
//     //         sphere_factor = 0.0;

//     //     return dense * sphere_factor;
//     // }

//     // stage 4 //

//     // {
//     //     Float sphere_factor =
//     //         std::sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
//     //     if (sphere_factor > 0.4 && sphere_factor < 0.55)
//     //         sphere_factor = 1.0;
//     //     else if (sphere_factor < 0.4 && sphere_factor > 0.20)
//     //         sphere_factor = (0.4 - sphere_factor) / 0.20;
//     //     else
//     //         sphere_factor = 0.0;

//     //     return dense * sphere_factor;
//     // }

//     // stage 5 //

//     // {
//     //     Float sphere_factor =
//     //         std::sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
//     //     if (sphere_factor > 0.4 && sphere_factor < 0.55)
//     //         sphere_factor = 1.0;
//     //     else if (sphere_factor < 0.4 && sphere_factor > 0.20)
//     //         sphere_factor = (0.4 - sphere_factor) / 0.20;
//     //     else if (sphere_factor < 0.20) {
//     //         sphere_factor = 0.0;
//     //     } else {
//     //         sphere_factor = (sqrt(0.75) - sphere_factor) / (sqrt(0.75) -
//     //         0.55); sphere_factor *= sphere_factor; sphere_factor *=
//     //         sphere_factor;
//     //     }

//     //     return dense * sphere_factor;
//     // }

//     // stage 6 //

//     // {
//     //     Float sphere_factor =
//     //         std::sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
//     //     if (sphere_factor > 0.4 && sphere_factor < 0.55)
//     //         sphere_factor = 1.0;
//     //     else if (sphere_factor < 0.4 && sphere_factor > 0.20)
//     //         sphere_factor = (0.4 - sphere_factor) / 0.20;
//     //     else if (sphere_factor < 0.20) {
//     //         sphere_factor = 0.0;
//     //     } else {
//     //         sphere_factor = (sqrt(0.75) - sphere_factor) / (sqrt(0.75) -
//     //         0.55); sphere_factor *= sphere_factor; sphere_factor *=
//     //         sphere_factor;
//     //     }

//     //     // Float val = dense * sphere_factor;

//     //     Float x_tmp = p[0];
//     //     Float y_tmp = p[1];
//     //     Float z_tmp = p[2];

//     //     x_tmp = 0.3 *
//     //             imedit::pbrtPerlinNoise(p[0] * 20.0, p[1] * 20.0, p[2]
//     //             * 20.0);
//     //     y_tmp = 0.6 * imedit::pbrtPerlinNoise(p[0] * 5.0 + x_tmp,
//     //                                           p[1] * 5.0 + x_tmp,
//     //                                           p[2] * 5.0 + x_tmp);
//     //     z_tmp = 0.1 * imedit::pbrtPerlinNoise(p[0] * 100.0 + y_tmp,
//     //                                           p[1] * 100.0 + y_tmp,
//     //                                           p[2] * 100.0 + y_tmp);

//     //     Float val =
//     //         dense[0] * falloff * sphere_factor *
//     //         std::abs((std::abs(x_tmp) + std::abs(y_tmp) +
//     std::abs(z_tmp)));

//     //     return std::min(val, 3.0);

//     //     // return dense * sphere_factor;
//     // }

//     // // stage 7 //
//     //
//     // {
//     //     Float sphere_factor =
//     //         std::sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
//     //     if (sphere_factor > 0.4 && sphere_factor < 0.55)
//     //         sphere_factor = 1.0;
//     //     else if (sphere_factor < 0.4 && sphere_factor > 0.20)
//     //         sphere_factor = (0.4 - sphere_factor) / 0.20;
//     //     else if (sphere_factor < 0.20) {
//     //         sphere_factor = 0.0;
//     //     } else {
//     //         sphere_factor = (sqrt(0.75) - sphere_factor) / (sqrt(0.75) -
//     //         0.55); sphere_factor *= sphere_factor; sphere_factor *=
//     //         sphere_factor;
//     //     }
//     //
//     //     // Float val = dense * sphere_factor;
//     //
//     //     Float x_tmp = p[0];
//     //     Float y_tmp = p[1];
//     //     Float z_tmp = p[2];
//     //
//     //     x_tmp = 0.3 *
//     //             imedit::pbrtPerlinNoise(p[0] * 20.0, p[1] * 20.0, p[2]
//     //             * 20.0);
//     //     y_tmp = 0.6 * imedit::pbrtPerlinNoise(p[0] * 5.0 + x_tmp,
//     //                                           p[1] * 5.0 + x_tmp,
//     //                                           p[2] * 5.0 + x_tmp);
//     //     z_tmp = 0.1 * imedit::pbrtPerlinNoise(p[0] * 100.0 + y_tmp,
//     //                                           p[1] * 100.0 + y_tmp,
//     //                                           p[2] * 100.0 + y_tmp);
//     //
//     //     Float turb = 0.5 * imedit::turbulence(x_tmp, y_tmp, z_tmp, 2.0);
//     //
//     //     Float val =
//     //         2.0 * falloff * sphere_factor *
//     //         std::abs((std::abs(x_tmp) + std::abs(y_tmp) + std::abs(z_tmp))
//     -
//     //                  turb);
//     //
//     //     return std::min(val, 3.0);
//     //
//     //     // return dense * sphere_factor;
//     // }

//     // // stage 8 //
//     //
//     // {
//     //     Float sphere_factor =
//     //         std::sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
//     //     if (sphere_factor > 0.4 && sphere_factor < 0.55)
//     //         sphere_factor = 1.0;
//     //     else if (sphere_factor < 0.4 && sphere_factor > 0.20)
//     //         sphere_factor = (0.4 - sphere_factor) / 0.20;
//     //     else if (sphere_factor < 0.20) {
//     //         sphere_factor = 0.0;
//     //     } else {
//     //         sphere_factor = (sqrt(0.75) - sphere_factor) / (sqrt(0.75) -
//     //         0.55); sphere_factor *= sphere_factor; sphere_factor *=
//     //         sphere_factor;
//     //     }
//     //
//     //     // Float val = dense * sphere_factor;
//     //
//     //     Float x_tmp = p[0];
//     //     Float y_tmp = p[1];
//     //     Float z_tmp = p[2];
//     //
//     //     x_tmp = 0.3 *
//     //             imedit::pbrtPerlinNoise(p[0] * 20.0, p[1] * 20.0, p[2]
//     //             * 20.0);
//     //     y_tmp = 0.6 * imedit::pbrtPerlinNoise(p[0] * 5.0 + x_tmp,
//     //                                           p[1] * 5.0 + x_tmp,
//     //                                           p[2] * 5.0 + x_tmp);
//     //     z_tmp = 0.1 * imedit::pbrtPerlinNoise(p[0] * 100.0 + y_tmp,
//     //                                           p[1] * 100.0 + y_tmp,
//     //                                           p[2] * 100.0 + y_tmp);
//     //
//     //     Float turb = 0.5 * imedit::turbulence(x_tmp, y_tmp, z_tmp, 2.0);
//     //
//     //     Float exp_factor_1 =
//     //         exp(-3.0 * std::max(-p[0], 0.0) + -4.0 * std::max(p[2], 0.0));
//     //
//     //     Float val =
//     //         2.0 * falloff * sphere_factor * exp_factor_1 *
//     //         std::abs((std::abs(x_tmp) + std::abs(y_tmp) + std::abs(z_tmp))
//     -
//     //                  turb);
//     //
//     //     return std::min(val, 3.0);
//     //
//     //     // return dense * sphere_factor;
//     // }

//     // stage 9 //

//     {
//         Float sphere_factor =
//             std::sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
//         if (sphere_factor > 0.4 && sphere_factor < 0.55)
//             sphere_factor = 1.0;
//         else if (sphere_factor < 0.4 && sphere_factor > 0.20)
//             sphere_factor = (0.4 - sphere_factor) / 0.20;
//         else if (sphere_factor < 0.20) {
//             sphere_factor = 0.0;
//         } else {
//             sphere_factor = (sqrt(0.75) - sphere_factor) / (sqrt(0.75) -
//             0.55); sphere_factor *= sphere_factor; sphere_factor *=
//             sphere_factor;
//         }

//         // Float val = dense * sphere_factor;

//         Float x_tmp = p[0];
//         Float y_tmp = p[1];
//         Float z_tmp = p[2];

//         x_tmp = 0.3 *
//                 imedit::pbrtPerlinNoise(p[0] * 20.0, p[1] * 20.0, p[2]
//                 * 20.0);
//         y_tmp = 0.6 * imedit::pbrtPerlinNoise(p[0] * 5.0 + x_tmp,
//                                               p[1] * 5.0 + x_tmp,
//                                               p[2] * 5.0 + x_tmp);
//         z_tmp = 0.1 * imedit::pbrtPerlinNoise(p[0] * 100.0 + y_tmp,
//                                               p[1] * 100.0 + y_tmp,
//                                               p[2] * 100.0 + y_tmp);

//         Float turb = 0.5 * imedit::turbulence(x_tmp, y_tmp, z_tmp, 2.0);

//         Float exp_factor_1 =
//             exp(-3.0 * std::max(-p[0], 0.0) + -4.0 * std::max(p[2], 0.0));
//         Float exp_factor_2 =
//             exp(-4.0 * std::min(-p[0], 0.0) + -3.0 * std::min(p[2], 0.0));

//         Float val =
//             2.0 * falloff * sphere_factor * exp_factor_1 * exp_factor_2 *
//             std::abs((std::abs(x_tmp) + std::abs(y_tmp) + std::abs(z_tmp)) -
//                      turb);

//         return std::min(val, 3.0);
//     }

//     // Float sphere_factor = std::sqrt(p[0] * p[0] + p[1] * p[1] + p[2] *
//     p[2]);
//     // if (sphere_factor > 0.4 && sphere_factor < 0.55)
//     //     sphere_factor = 1.0;
//     // else if (sphere_factor < 0.4 && sphere_factor > 0.20)
//     //     sphere_factor = (0.4 - sphere_factor) / 0.20;
//     // else if (sphere_factor < 0.20) {
//     //     sphere_factor = 0.0;
//     // } else {
//     //     sphere_factor = (sqrt(0.75) - sphere_factor) / (sqrt(0.75) -
//     0.55);
//     //     sphere_factor *= sphere_factor;
//     //     sphere_factor *= sphere_factor;
//     // }

//     // Float x_tmp = p[0];
//     // Float y_tmp = p[1];
//     // Float z_tmp = p[2];

//     // x_tmp =
//     //     0.3 * imedit::pbrtPerlinNoise(p[0] * 20.0, p[1] * 20.0, p[2]
//     * 20.0);
//     // y_tmp =
//     //     0.6 * imedit::pbrtPerlinNoise(p[0] * 5.0 + x_tmp, p[1] * 5.0 +
//     x_tmp,
//     //                                   p[2] * 5.0 + x_tmp);
//     // z_tmp = 0.1 * imedit::pbrtPerlinNoise(p[0] * 100.0 + y_tmp,
//     //                                       p[1] * 100.0 + y_tmp,
//     //                                       p[2] * 100.0 + y_tmp);

//     // Float turb = 0.5 * imedit::turbulence(x_tmp, y_tmp, z_tmp, 2.0);

//     // Float sphere_factor = std::sqrt(p[0] * p[0] + p[1] * p[1] + p[2] *
//     p[2]);
//     // if (sphere_factor > 0.4 && sphere_factor < 0.55)
//     //     sphere_factor = 1.0;
//     // else if (sphere_factor < 0.4 && sphere_factor > 0.20)
//     //     sphere_factor = (0.4 - sphere_factor) / 0.20;
//     // else if (sphere_factor < 0.20) {
//     //     sphere_factor = 0.0;
//     // } else {
//     //     sphere_factor = (sqrt(0.75) - sphere_factor) / (sqrt(0.75) -
//     0.55);
//     //     sphere_factor *= sphere_factor;
//     //     sphere_factor *= sphere_factor;
//     // }

//     // Float exp_factor_1 =
//     //     exp(-3.0 * std::max(-p[0], 0.0) + -4.0 * std::max(p[2], 0.0));
//     // Float exp_factor_2 =
//     //     exp(-4.0 * std::min(-p[0], 0.0) + -3.0 * std::min(p[2], 0.0));

//     // Float val =
//     //     2.0 * sphere_factor * falloff * exp_factor_1 * exp_factor_2 *
//     //     std::abs((std::abs(x_tmp) + std::abs(y_tmp) + std::abs(z_tmp)) -
//     //     turb);

//     // return std::min(val, 3.0);
// }

// Spectrum ProceduralSphereDensity::D(const Point3f& p, bool buildState) const
// {
//     Point3f base = p;
//     Point3f one = p + Point3f(0.01f, 0.0f, 0.0f);
//     Point3f two = p + Point3f(-0.01f, 0.0f, 0.0f);
//     Point3f three = p + Point3f(0.0f, 0.01f, 0.0f);
//     Point3f four = p + Point3f(0.0f, -0.01f, 0.0f);

//     // return proc_D(base, buildState);

//     // return 0.2 * (proc_D(base, buildState) + proc_D(one, buildState) +
//     //               proc_D(three, buildState) + proc_D(two, buildState) +
//     //               proc_D(four, buildState));
// }

// Spectrum ProceduralSphereDensity::getMaxDensity() const { return dense; }

// Spectrum ProceduralSphereDensity::getMinDensity() const { return dense; }

// Spectrum ProceduralSphereDensity::getMaxDensityInRange(const Point3f& a,
//                                                        const Point3f& b)
//                                                        const {
//     return dense;
// }

// Spectrum ProceduralSphereDensity::getMinDensityInRange(const Point3f& a,
//                                                        const Point3f& b)
//                                                        const {
//     return dense;
// }

// }  // namespace pbrt
