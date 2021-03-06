/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2015 Alberto Passalacqua
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is derivative work of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "AyaziShamlou.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace populationBalanceSubModels
{
namespace breakupKernels
{
    defineTypeNameAndDebug(AyaziShamlou, 0);

    addToRunTimeSelectionTable
    (
        breakupKernel,
        AyaziShamlou,
        dictionary
    );
}
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::populationBalanceSubModels::breakupKernels::AyaziShamlou
::AyaziShamlou
(
    const dictionary& dict,
    const fvMesh& mesh
)
:
    breakupKernel(dict, mesh),
    A_(dict.lookup("A")),
    df_(dict.lookup("df")),
    H0_(dict.lookup("H0")),
    primarySize_(dict.lookup("primarySize")),
    flThermo_(mesh_.lookupObject<fluidThermo>(basicThermo::dictName)),
    flTurb_
    (
        mesh_.lookupObject<compressible::turbulenceModel>
        (
            turbulenceModel::propertiesName
        )
    ),
    epsilon_(flTurb_.epsilon()),
    mu_(flThermo_.mu()),
    rho_(flThermo_.rho())
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::populationBalanceSubModels::breakupKernels::AyaziShamlou::~AyaziShamlou()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::scalar
Foam::populationBalanceSubModels::breakupKernels::AyaziShamlou::Kb
(
    const scalar& abscissa,
    const label celli
) const
{
    // Interparticle force
    scalar F = A_.value()*primarySize_.value()/(12.0*sqr(H0_.value()));

    // Coefficient of volume fraction (Vanni, 2000)
    scalar C = 0.41*df_.value() - 0.211;

    // Volume fraction of solid within aggregates
    scalar phiL = C*pow(abscissa/primarySize_.value(), df_.value() - 3.0);

    // Coordination number
    scalar kc = 15.0*pow(phiL, 1.2);

    // Aggregation strength
    scalar sigma = 9.0*kc*phiL*F/(8.0*sqr(primarySize_.value())
            *Foam::constant::mathematical::pi);

    scalar epsilonByNu = epsilon_[celli]*rho_[celli]/mu_[celli];

    scalar tau = mu_[celli]*sqrt(epsilonByNu);

    return sqrt(epsilonByNu/15.0)*exp(-sigma/tau);
}

// ************************************************************************* //
