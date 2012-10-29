#include "transform.hpp"
const Transform Transform::IDENTITY;

// Set the initial matrix to identity matrix
// Translate to (0.0,0.0,0.0) and scale to (1.0,1.0,1.0)
Transform::Transform ()
    :
    mHMatrix(1.0f),
    mInvHMatrix(1.0f),
    mMatrix(1.0f),
    mTranslate(0.0f),
    mScale(1.0f),
    mIsIdentity(true),
    mIsRSMatrix(true),
    mIsUniformScale(true),
    mInverseNeedsUpdate(false)
{
}

Transform::~Transform ()
{
}

void Transform::MakeIdentity ()
{
    mHMatrix=glm::mat4(1.0f);
    mInvHMatrix=glm::mat4(1.0f);
    mMatrix = glm::mat3(1.0f);
    mTranslate = glm::vec3(0.0f);
    mScale = glm::vec3(1.0f);
    mIsIdentity = true;
    mIsRSMatrix = true;
    mIsUniformScale = true;
    mInverseNeedsUpdate=false;
}

void Transform::MakeUnitScale ()
{
    assertion(mIsRSMatrix, "Matrix is not a rotation\n");

    mScale = glm::vec3(1.0f);
    mIsUniformScale = true;
    UpdateHMatrix();
}

void Transform::SetRotate (const glm::mat3& rotate)
{
    mMatrix = rotate;
    mIsIdentity = false;
    mIsRSMatrix = true;
    UpdateHMatrix();
}

void Transform::SetMatrix (const glm::mat3& matrix)
{
    mMatrix = matrix;
    mIsIdentity = false;
    mIsRSMatrix = false;
    mIsUniformScale = false;
    UpdateHMatrix();
}

void Transform::SetTranslate (const glm::vec3& translate)
{
    mTranslate = translate;
    mIsIdentity = false;
    UpdateHMatrix();
}

void Transform::SetScale (const glm::vec3& scale)
{
    assertion(mIsRSMatrix, "Matrix is not a rotation\n");
    assertion(scale[0] != 0.0f && scale[1] != 0.0f && scale[2] != 0.0f,
        "Scales must be nonzero\n");

    mScale = scale;
    mIsIdentity = false;
    mIsUniformScale = false;
    UpdateHMatrix();
}

void Transform::SetUniformScale (float scale)
{
    assertion(mIsRSMatrix, "Matrix is not a rotation\n");
    assertion(scale != 0.0f, "Scale must be nonzero\n");

    mScale = glm::vec3(scale);
    mIsIdentity = false;
    mIsUniformScale = true;
    UpdateHMatrix();
}
//----------------------------------------------------------------------------
float Transform::GetNorm () const
{
    if (mIsRSMatrix)
    {
        float maxValue = glm::abs(mScale[0]);
        float curValue = glm::abs(mScale[1]);
        if (curValue > maxValue)
        {
            maxValue = curValue;
        }
        curValue = glm::abs(mScale[2]);
        if (curValue > maxValue)
        {
            maxValue = curValue;
        }
        return maxValue;
    }

    // A general matrix.  Use the max-row-sum matrix norm.  The spectral
    // norm (the maximum absolute value of the eigenvalues) is smaller or
    // equal to this norm.  Therefore, this function returns an approximation
    // to the maximum scale.

    // Because glm use column major matrix, change the code
    float maxRowSum =
        glm::abs(mMatrix[0][0]) +
        glm::abs(mMatrix[1][0]) +
        glm::abs(mMatrix[2][0]);

    float rowSum =
        glm::abs(mMatrix[0][1]) +
        glm::abs(mMatrix[1][1]) +
        glm::abs(mMatrix[2][1]);

    if (rowSum > maxRowSum)
    {
        maxRowSum = rowSum;
    }

    rowSum =
        glm::abs(mMatrix[0][2]) +
        glm::abs(mMatrix[1][2]) +
        glm::abs(mMatrix[2][2]);

    if (rowSum > maxRowSum)
    {
        maxRowSum = rowSum;
    }

    return maxRowSum;
}
//----------------------------------------------------------------------------
Transform Transform::operator* (const Transform& transform) const
{
    if (IsIdentity())
    {
        return transform;
    }

    if (transform.IsIdentity())
    {
        return *this;
    }

    Transform product;

    if (mIsRSMatrix && transform.mIsRSMatrix)
    {
        if (mIsUniformScale)
        {
            product.SetRotate(mMatrix*transform.mMatrix);

            product.SetTranslate(GetUniformScale()*(
                mMatrix*transform.mTranslate) + mTranslate);

            if (transform.IsUniformScale())
            {
                product.SetUniformScale(
                    GetUniformScale()*transform.GetUniformScale());
            }
            else
            {
                product.SetScale(GetUniformScale()*transform.GetScale());
            }

            return product;
        }
    }

    // In all remaining cases, the matrix cannot be written as R*S*X+T.
    glm::mat3 matMA = (mIsRSMatrix ?
        glm::mat3(mMatrix[0]*mScale[0],mMatrix[1]*mScale[1],
                  mMatrix[2]*mScale[2]): mMatrix);
    glm::mat3 matMB = (transform.mIsRSMatrix ?
        glm::mat3(transform.mMatrix[0]*transform.mScale[0],
                  transform.mMatrix[1]*transform.mScale[1],
                  transform.mMatrix[2]*transform.mScale[2]): mMatrix);

    product.SetMatrix(matMA*matMB);
    product.SetTranslate(matMA*transform.mTranslate + mTranslate);
    return product;
}
//----------------------------------------------------------------------------
const glm::mat4& Transform::Inverse () const
{
    if (mInverseNeedsUpdate)
    {
        if (mIsIdentity)
        {
            mInvHMatrix = glm::mat4(1.0);
        }
        else
        {
            if (mIsRSMatrix)
            {
                if (mIsUniformScale)
                {
                    float invScale = 1.0f/mScale[0];
                    /*
                    mInvHMatrix[0][0] = invScale*mMatrix[0][0];
                    mInvHMatrix[0][1] = invScale*mMatrix[1][0];
                    mInvHMatrix[0][2] = invScale*mMatrix[2][0];
                    mInvHMatrix[1][0] = invScale*mMatrix[0][1];
                    mInvHMatrix[1][1] = invScale*mMatrix[1][1];
                    mInvHMatrix[1][2] = invScale*mMatrix[2][1];
                    mInvHMatrix[2][0] = invScale*mMatrix[0][2];
                    mInvHMatrix[2][1] = invScale*mMatrix[1][2];
                    mInvHMatrix[2][2] = invScale*mMatrix[2][2];*/
                    mInvHMatrix=glm::mat4(invScale*glm::transpose(mMatrix));
                }
                else
                {
                    // Replace 3 reciprocals by 6 multiplies and 1 reciprocal.
                    float s01 = mScale[0]*mScale[1];
                    float s02 = mScale[0]*mScale[2];
                    float s12 = mScale[1]*mScale[2];
                    float invs012 = 1.0f/(s01*mScale[2]);
                    float invS0 = s12*invs012;
                    float invS1 = s02*invs012;
                    float invS2 = s01*invs012;
                    /*mInvHMatrix[0][0] = invS0*mMatrix[0][0];
                    mInvHMatrix[0][1] = invS0*mMatrix[1][0];
                    mInvHMatrix[0][2] = invS0*mMatrix[2][0];
                    mInvHMatrix[1][0] = invS1*mMatrix[0][1];
                    mInvHMatrix[1][1] = invS1*mMatrix[1][1];
                    mInvHMatrix[1][2] = invS1*mMatrix[2][1];
                    mInvHMatrix[2][0] = invS2*mMatrix[0][2];
                    mInvHMatrix[2][1] = invS2*mMatrix[1][2];
                    mInvHMatrix[2][2] = invS2*mMatrix[2][2];*/

                    mInvHMatrix[0][0] = invS0*mMatrix[0][0];
                    mInvHMatrix[0][1] = invS1*mMatrix[1][0];
                    mInvHMatrix[0][2] = invS2*mMatrix[2][0];
                    mInvHMatrix[1][0] = invS0*mMatrix[0][1];
                    mInvHMatrix[1][1] = invS1*mMatrix[1][1];
                    mInvHMatrix[1][2] = invS2*mMatrix[2][1];
                    mInvHMatrix[2][0] = invS0*mMatrix[0][2];
                    mInvHMatrix[2][1] = invS1*mMatrix[1][2];
                    mInvHMatrix[2][2] = invS2*mMatrix[2][2];

                }
            }
            else
            {
//                mInvHMatrix=glm::inverse(mHMatrix);
//                mInverseNeedsUpdate = false;
//                return mInvHMatrix;
                mInvHMatrix=glm::mat4(glm::inverse(glm::mat3(mHMatrix)));
            }
//todo
            /*mInvHMatrix[3][0] = -(
                mInvHMatrix[0][0]*mTranslate[0] +
                mInvHMatrix[0][1]*mTranslate[1] +
                mInvHMatrix[0][2]*mTranslate[2]
            );

            mInvHMatrix[3][1] = -(
                mInvHMatrix[1][0]*mTranslate[0] +
                mInvHMatrix[1][1]*mTranslate[1] +
                mInvHMatrix[1][2]*mTranslate[2]
            );

            mInvHMatrix[3][2] = -(
                mInvHMatrix[2][0]*mTranslate[0] +
                mInvHMatrix[2][1]*mTranslate[1] +
                mInvHMatrix[2][2]*mTranslate[2]
            );*/
            mInvHMatrix[3]=mInvHMatrix*glm::vec4(mTranslate,0.0f);

            // The last row of mHMatrix is always (0,0,0,1) for an affine
            // transformation, so it is set once in the constructor.  It is
            // not necessary to reset it here.
        }

        mInverseNeedsUpdate = false;
    }

    return mInvHMatrix;
}
//----------------------------------------------------------------------------
Transform Transform::InverseTransform () const
{
    if (mIsIdentity)
    {
        return IDENTITY;
    }

    Transform inverse;
    glm::vec3 invTrn;
    if (mIsRSMatrix)
    {
        glm::mat3 invRot = glm::transpose(mMatrix);
        inverse.SetRotate(invRot);
        if (mIsUniformScale)
        {
            float invScale = 1.0f/mScale[0];
            inverse.SetUniformScale(invScale);
            invTrn = -invScale*(invRot*mTranslate);
        }
        else
        {
            glm::vec3 invScale(1.0f/mScale[0], 1.0f/mScale[1], 1.0f/mScale[2]);
            inverse.SetScale(invScale);
            invTrn = invRot*mTranslate;
            invTrn[0] *= -invScale[0];
            invTrn[1] *= -invScale[1];
            invTrn[2] *= -invScale[2];
        }
    }
    else
    {
        glm::mat3 invMat=glm::inverse(mMatrix);
        inverse.SetMatrix(invMat);
        invTrn = -(invMat*mTranslate);
    }
    inverse.SetTranslate(invTrn);

    return inverse;
}
//----------------------------------------------------------------------------
void Transform::UpdateHMatrix ()
{
    if (mIsIdentity)
    {
        mHMatrix = glm::mat4(1.0f);
    }
    else
    {
        if (mIsRSMatrix)
        {
            /*mHMatrix[0][0] = mMatrix[0][0]*mScale[0];
            mHMatrix[0][1] = mMatrix[0][1]*mScale[1];
            mHMatrix[0][2] = mMatrix[0][2]*mScale[2];
            mHMatrix[1][0] = mMatrix[1][0]*mScale[0];
            mHMatrix[1][1] = mMatrix[1][1]*mScale[1];
            mHMatrix[1][2] = mMatrix[1][2]*mScale[2];
            mHMatrix[2][0] = mMatrix[2][0]*mScale[0];
            mHMatrix[2][1] = mMatrix[2][1]*mScale[1];
            mHMatrix[2][2] = mMatrix[2][2]*mScale[2];*/
            mHMatrix[0]=mHMatrix[0]*mScale[0];
            mHMatrix[1]=mHMatrix[1]*mScale[1];
            mHMatrix[2]=mHMatrix[2]*mScale[2];
        }
        else
        {
            /*mHMatrix[0][0] = mMatrix[0][0];
            mHMatrix[0][1] = mMatrix[0][1];
            mHMatrix[0][2] = mMatrix[0][2];
            mHMatrix[1][0] = mMatrix[1][0];
            mHMatrix[1][1] = mMatrix[1][1];
            mHMatrix[1][2] = mMatrix[1][2];
            mHMatrix[2][0] = mMatrix[2][0];
            mHMatrix[2][1] = mMatrix[2][1];
            mHMatrix[2][2] = mMatrix[2][2];*/
            mHMatrix=glm::mat4(mMatrix);
        }

        mHMatrix[3][0] = mTranslate[0];
        mHMatrix[3][1] = mTranslate[1];
        mHMatrix[3][2] = mTranslate[2];

        // The last row of mHMatrix is always (0,0,0,1) for an affine
        // transformation, so it is set once in the constructor.  It is not
        // necessary to reset it here.
    }

    mInverseNeedsUpdate = true;
}
