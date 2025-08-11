#include "LabelSegments.hpp"

// API
#include "ACAPinc.h"

// ACAPI
#include "ACAPI/Result.hpp"

// MEPAPI
#include "ACAPI/MEPAdapter.hpp"
#include "ACAPI/MEPRoutingElement.hpp"

// STL
#include <vector>


using namespace ACAPI::MEP;


namespace MEPExample {


GSErrCode LabelSegmentsOfSelectedRouteElements ()
{
	GS::Array<API_Neig> selNeigs;
	API_SelectionInfo	selectionInfo = {};
	ERRCHK_NO_ASSERT (ACAPI_Selection_Get (&selectionInfo, &selNeigs, false, false));
	
	return ACAPI_CallUndoableCommand ("Label Segments of Selected Route Elements", [&] () -> GSErrCode {
		for (const API_Neig& neig : selNeigs) {
			API_Element elem = {};
			elem.header.guid = neig.guid;
			ERRCHK (ACAPI_Element_Get (&elem));

			if (elem.header.type.typeID != API_ExternalElemID || !IsRoutingElement (elem.header.type.classID))
				continue;

// ! [RoutingSegment-Labeling-Example]
			// The following code snippet places an associative (autotext) Label on each RouteSegment of a RoutingElement.
			// The label will be associative to the middle point of the RouteSegment (createAtDefaultPosition = true).
			// The content of these Labels will be taken from the current Label default.
			// The RoutingElement and RoutingNode elements are not labelable.

			const ACAPI::Result<RoutingElement> routingElement = RoutingElement::Get (Adapter::UniqueID (neig.guid));
			if (routingElement.IsErr ()) {
				ACAPI_WriteReport (routingElement.UnwrapErr ().text.c_str (), false);
				return routingElement.UnwrapErr ().kind;
			}

			for (const UniqueID& segmentId : routingElement->GetRoutingSegmentIds ()) {
				bool autoTextFlag;
				ERRCHK (ACAPI_AutoText_GetAutoTextFlag (&autoTextFlag));

				if (autoTextFlag) {
					bool autoTextFlagOff = false;
					ERRCHK (ACAPI_AutoText_ChangeAutoTextFlag (&autoTextFlagOff));
				}

				API_Element element = {};
				element.header.type = API_LabelID;

				API_ElementMemo memo = {};

				ERRCHK (ACAPI_Element_GetDefaults (&element, &memo));
	
				if (autoTextFlag) {
					ERRCHK (ACAPI_AutoText_ChangeAutoTextFlag (&autoTextFlag));
				}

				element.label.createAtDefaultPosition = true;
				element.label.parentType			  = API_ExternalElemID;
				element.label.parent				  = GSGuid2APIGuid (segmentId.GetGuid ());

				ERRCHK (ACAPI_Element_Create (&element, &memo));

				ERRCHK (ACAPI_DisposeElemMemoHdls (&memo));
			}
// ! [RoutingSegment-Labeling-Example]
		}

		return NoError;
	});
}


} // namespace MEPExample
