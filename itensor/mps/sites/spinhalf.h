//
// Distributed under the ITensor Library License, Version 1.2
//    (See accompanying LICENSE file.)
//
#ifndef __ITENSOR_SPINHALF_H
#define __ITENSOR_SPINHALF_H
#include "itensor/mps/siteset.h"

namespace itensor {

class SpinHalfSite;

using SpinHalf = BasicSiteSet<SpinHalfSite>;

class SpinHalfSite
    {
    Index s;
    public:

    SpinHalfSite() { }

    SpinHalfSite(Index const& I) : s(I) { }

    SpinHalfSite(int n, Args const& args = Args::global())
        {
        auto conserveqn = args.getBool("ConserveQNs",false);
        auto conserveSz = args.getBool("ConserveSz",false);
        if(conserveqn || conserveSz)
            {
            s = Index{nameint("S=1/2 ",n),
                      QN("Sz=",+1),1,
                      QN("Sz=",-1),1,Out,Site};
            }
        else
            {
            s = Index{nameint("S=1/2 ",n),2,Site};
            }
        }

    Index
    index() const { return s; }

    IndexVal
    state(std::string const& state)
        {
        if(state == "Up") 
            {
            return s(1);
            }
        else 
        if(state == "Dn") 
            {
            return s(2);
            }
        else
            {
            Error("State " + state + " not recognized");
            }
        return IndexVal{};
        }

	ITensor
	op(std::string const& opname,
	   Args const& args) const
        {
        auto sP = prime(s);

        auto Up = s(1);
        auto UpP = sP(1);
        auto Dn = s(2);
        auto DnP = sP(2);

        auto Op = ITensor(dag(s),sP);

        if(opname == "Sz")
            {
            Op.set(Up,UpP,+0.5);
            Op.set(Dn,DnP,-0.5);
            }
        else
        if(opname == "Sx")
            {
            Op.set(Up,DnP,+0.5);
            Op.set(Dn,UpP,+0.5);
            }
        else
        if(opname == "ISy")
            {
            Op.set(Up,DnP,-0.5);
            Op.set(Dn,UpP,+0.5);
            }
        else
        if(opname == "Sy")
            {
            Op.set(Up,DnP,+0.5*Cplx_i);
            Op.set(Dn,UpP,-0.5*Cplx_i);
            }
        else
        if(opname == "Sp" || opname == "S+")
            {
            Op.set(Dn,UpP,1);
            }
        else
        if(opname == "Sm" || opname == "S-")
            {
            Op.set(Up,DnP,1);
            }
        else
        if(opname == "projUp")
            {
            Op.set(Up,UpP,1);
            }
        else
        if(opname == "projDn")
            {
            Op.set(Dn,DnP,1);
            }
        else
        if(opname == "S2")
            {
            Op.set(Up,UpP,0.75);
            Op.set(Dn,DnP,0.75);
            }
        else
            {
            Error("Operator \"" + opname + "\" name not recognized");
            }

        return Op;
        }
    };

} //namespace itensor

#endif
