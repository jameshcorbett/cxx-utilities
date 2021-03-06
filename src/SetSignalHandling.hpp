/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Copyright (c) 2019, Lawrence Livermore National Security, LLC.
 *
 * Produced at the Lawrence Livermore National Laboratory
 *
 * LLNL-CODE-746361
 *
 * All rights reserved. See COPYRIGHT for details.
 *
 * This file is part of the GEOSX Simulation Framework.
 *
 * GEOSX is a free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License (as published by the
 * Free Software Foundation) version 2.1 dated February 1999.
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/**
 * @file SetSignalHandling.hpp
 */

#ifndef COMPONENTS_CORE_SRC_CODINGUTILITIES_SETSIGNALHANDLING_HPP_
#define COMPONENTS_CORE_SRC_CODINGUTILITIES_SETSIGNALHANDLING_HPP_

#include <exception>

namespace LvArray
{

/**
 * @brief Set the signal handler for common signals.
 * @param handler The signal handler.
 */
void setSignalHandling( void (* handler)( int ) );

/**
 * @brief Rest the signal handling back to the original state.
 */
void resetSignalHandling();

} /* namespace geosx */

#endif /* COMPONENTS_CORE_SRC_CODINGUTILITIES_SETSIGNALHANDLING_HPP_ */
