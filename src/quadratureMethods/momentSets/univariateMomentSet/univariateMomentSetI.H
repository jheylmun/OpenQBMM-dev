/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2014-2017 Alberto Passalacqua
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

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::univariateMomentSet::isDegenerate()
{
    // Checking if moments of order higher than zero are null
    // The case when m0 = 0 is already managed
    for (Foam::label momentI = 1; momentI < (*this).size(); momentI++)
    {
        if (mag((*this)[momentI]) > SMALL)
        {
            degenerate_ = false;

            return degenerate_;
        }
    }

    degenerate_ = true;

    return degenerate_;
}

bool Foam::univariateMomentSet::isFullyRealizable()
{
    if (!realizabilityChecked_)
    {
        checkRealizability();
    }

    return fullyRealizable_;
}

bool Foam::univariateMomentSet::isSubsetRealizable()
{
    if (!realizabilityChecked_)
    {
        checkRealizability();
    }

    return subsetRealizable_;
}

bool Foam::univariateMomentSet::isOnMomentSpaceBoundary()
{
    if (!realizabilityChecked_)
    {
        checkRealizability();
    }

    return onMomentSpaceBoundary_;
}

Foam::label Foam::univariateMomentSet::nRealizableMoments()
{
    if (!realizabilityChecked_)
    {
        checkRealizability();
    }

    return nRealizableMoments_;
}

// ************************************************************************* //