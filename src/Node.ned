//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package cmpn405_project;

//
// TODO auto-generated module
//
simple Node
{
    parameters:
    	double delay = default(0.2);
    	double timer = default(1);
    	int windowSize = default(2);
    	int errorCheckingAlgorithm = default(0);
    gates:
        input coord;
        input in;
        output out;
}
