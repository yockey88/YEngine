#include "Math/math.hpp"

#include <glm/gtx/matrix_decompose.hpp>

namespace machy {
namespace math {

    bool decompose(const glm::mat4& transform , glm::vec3& translation , glm::vec3& rotation , glm::vec3& scale) {

        /* Taken from glm/gtx/matrix_decompose.inl */
        using namespace glm;
        using T = float;

        mat4 LocalMatrix(transform);

        // Normalize the matrix.
        if(epsilonEqual(LocalMatrix[3][3], static_cast<T>(0), epsilon<T>()))
			return false;

        if(epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>())) {

			// Clear the perspective partition
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
			LocalMatrix[3][3] = static_cast<T>(1);
		}

        // Next take care of translation (easy).
		translation = vec3(LocalMatrix[3]);
		LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

		vec3 Row[3];

		// Now get scale and shear.
		for (length_t i = 0; i < 3; ++i)
			for (length_t j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];

		// Compute X scale factor and normalize first row.
		scale.x = length(Row[0]);
		Row[0] = detail::scale(Row[0], static_cast<T>(1));
		scale.y = length(Row[1]);
		Row[1] = detail::scale(Row[1], static_cast<T>(1));
		scale.z = length(Row[2]);
		Row[2] = detail::scale(Row[2], static_cast<T>(1));

		// At this point, the matrix (in rows[]) is orthonormal.
		// Check for a coordinate system flip.  If the determinant
		// is -1, then negate the matrix and the scaling factors.


        /*
            // At this point, the matrix (in rows[]) is orthonormal.
            // Check for a coordinate system flip.  If the determinant
            // is -1, then negate the matrix and the scaling factors.
            Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
            if(dot(Row[0], Pdum3) < 0)
            {
                for(length_t i = 0; i < 3; i++)
                {
                    Scale[i] *= static_cast<T>(-1);
                    Row[i] *= static_cast<T>(-1);
                }
            }
        */

		// Now, get the rotations out, as described in the gem.

		// FIXME - Add the ability to return either quaternions (which are
		// easier to recompose with) or Euler angles (rx, ry, rz), which
		// are easier for authors to deal with. The latter will only be useful
		// when we fix https://bugs.webkit.org/show_bug.cgi?id=23799, so I
		// will leave the Euler angle code here for now.
		rotation.y = asin(-Row[0][2]);
		if (cos(rotation.y) != 0) {
		    rotation.x = atan2(Row[1][2], Row[2][2]);
		    rotation.z = atan2(Row[0][1], Row[0][0]);
		} else {
		    rotation.x = atan2(-Row[2][0], Row[1][1]);
		    rotation.z = 0;
		}

        return true;
    }

}
}