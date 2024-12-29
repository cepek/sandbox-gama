/*
    GNU Gama -- adjustment of geodetic networks
    Copyright (C) 2003  Ales Cepek <cepek@gnu.org>

    This file is part of the GNU Gama C++ Library.

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef GNU_gama_xml_expat_h_GNU_Gama_interface_for_Expat_XML_parser
#define GNU_gama_xml_expat_h_GNU_Gama_interface_for_Expat_XML_parser


#ifdef  GNU_gama_expat_1_1

/* If for any reason you don't have expat parser installed you can
 * still build GNU Gama using old expat version 1.1 */
#include <expat/xmlparse/xmlparse.h>

// https://libexpat.github.io/doc/api/latest/#XML_ExpatVersion
//
//    XML_MAJOR_VERSION, XML_MINOR_VERSION, XML_MICRO_VERSION
//
// Testing these constants is currently the best way to determine
// if particular parts of the Expat API are available.

#define XML_MAJOR_VERSION 1
#define XML_MINOR_VERSION 1
#define XML_MICRO_VERSION 0

#else

/* For expat 1.95.2 or any later */
#include <expat.h>

#endif


#endif
