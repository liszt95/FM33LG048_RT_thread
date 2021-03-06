/**
  *******************************************************************************************************
  * @file    fm33lg0xx_fl_pgl.c
  * @author  FMSH Application Team
  * @brief   Src file of PGL FL Module
  *******************************************************************************************************
  * @attention
  *
  * Copyright (c) [2019] [Fudan Microelectronics]
  * THIS SOFTWARE is licensed under the Mulan PSL v1.
  * can use this software according to the terms and conditions of the Mulan PSL v1.
  * You may obtain a copy of Mulan PSL v1 at:
  * http://license.coscl.org.cn/MulanPSL
  * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
  * PURPOSE.
  * See the Mulan PSL v1 for more details.
  *
  *******************************************************************************************************
  */
#include "fm33lg0xx_fl_pgl.h"
#include "fm33lg0xx_fl_cmu.h"
#include "fm33lg0xx_fl_dma.h"
#include "fm33lg0xx_fl_rmu.h"
#include "fm33_assert.h"
/* Private macros ------------------------------------------------------------*/
/** @addtogroup LUT_FL_Private_Macros
  * @{
  */

#define         IS_FL_PGL_LUT_INSTANCE(INSTANCE)                ((INSTANCE) == PGL)

#define         IS_FL_PGL_LUT_EDGE(__VALUE__)              (((__VALUE__) == FL_PGL_LUT_EDGE_RISING)||\
                                                            ((__VALUE__) == FL_PGL_LUT_EDGE_FALLING)||\
                                                            ((__VALUE__) == FL_PGL_LUT_EDGE_BOTH)||\
                                                            ((__VALUE__) == FL_PGL_LUT_EDGE_NONE))


#define         IS_FL_PGL_LUT_OUT(__VALUE__)              (((__VALUE__) == FL_PGL_LUT_OUTPUT_COMBINE_LOGIC)||\
                                                           ((__VALUE__) == FL_PGL_LUT_OUTPUT_SYNC_SAMPLING)||\
                                                           ((__VALUE__) == FL_PGL_LUT_OUTPUT_DIGITAL_FILTER))

#define         IS_FL_PGL_LUT_INPUTSEL(__VALUE__)          (((__VALUE__) == FL_PGL_LUT_INPUT_GROUP0)||\
                                                            ((__VALUE__) == FL_PGL_LUT_INPUT_GROUP1)||\
                                                            ((__VALUE__) == FL_PGL_LUT_INPUT_GROUP2)||\
                                                            ((__VALUE__) == FL_PGL_LUT_INPUT_GROUP3)||\
                                                            ((__VALUE__) == FL_PGL_LUT_INPUT_DISABLE))


/**
  * @brief  ????????????PGL?????????.
  * @param  PGLx
  * @retval ErrorStatus?????????:
  *         -FL_PASS ?????????????????????????????????
  *         -FL_FAIL ???????????????
  */
FL_ErrorStatus FL_PGL_DeInit(PGL_Type *PGLx)
{
    assert_param(IS_FL_PGL_LUT_INSTANCE(PGLx));
    /* ?????????????????? */
    FL_RMU_EnablePeripheralReset(RMU);
    /* ??????PGL??????????????? */
    FL_RMU_EnableResetAPBPeripheral(RMU, FL_RMU_RSTAPB_PGL);
    FL_RMU_DisableResetAPBPeripheral(RMU, FL_RMU_RSTAPB_PGL);
    /* ??????????????????????????????????????? */
    FL_CMU_DisableGroup2BusClock(FL_CMU_GROUP2_BUSCLK_PGL);
    /* ?????????????????? */
    FL_RMU_DisablePeripheralReset(RMU);
    return FL_PASS;
}

/**
  * @brief  ?????? lutInit ???????????????????????????????????????????????????????????????
  * @param  PGLx PGLx
  * @param  lutInit ???????????? @ref FL_PGL_LUT_InitTypeDef ?????????
  *         ??????????????????????????????????????????.
  * @retval ErrorStatus?????????
  *         -FL_FAIL ????????????????????????
  *         -FL_PASS ????????????
  */
FL_ErrorStatus FL_PGL_LUT_Init(PGL_Type *PGLx, uint32_t LUTx, FL_PGL_LUT_InitTypeDef *lutInit)
{
    /* ?????????????????? */
    assert_param(IS_FL_PGL_LUT_INSTANCE(PGLx));
    assert_param(IS_FL_PGL_LUT_EDGE(lutInit->edge));
    assert_param(IS_FL_PGL_LUT_OUT(lutInit->outMode));
    assert_param(IS_FL_PGL_LUT_INPUTSEL(lutInit->input3));
    assert_param(IS_FL_PGL_LUT_INPUTSEL(lutInit->input2));
    assert_param(IS_FL_PGL_LUT_INPUTSEL(lutInit->input1));
    assert_param(IS_FL_PGL_LUT_INPUTSEL(lutInit->input0));
    /* ?????????????????? */
    FL_CMU_EnableGroup2BusClock(FL_CMU_GROUP2_BUSCLK_PGL);
    /*??????????????????*/
    FL_PGL_SetLUTEffectiveEdge(PGLx, LUTx, lutInit->edge);
    /*??????????????????*/
    FL_PGL_SetLUTOutputMode(PGLx, LUTx, lutInit->outMode);
    /*??????3??????*/
    if(lutInit->input3 == FL_PGL_LUT_INPUT_DISABLE)
    {
        FL_PGL_SetLUTInputSource(PGLx, LUTx, FL_PGL_INPUT3, FL_PGL_LUT_INPUT_GROUP0);
        FL_PGL_EnableLUTInputMask(PGLx, LUTx, FL_PGL_INPUT3); //????????????3??????
    }
    else
    {
        FL_PGL_SetLUTInputSource(PGLx, LUTx, FL_PGL_INPUT3, lutInit->input3);
    }
    /*??????2??????*/
    if(lutInit->input2 == FL_PGL_LUT_INPUT_DISABLE)
    {
        FL_PGL_SetLUTInputSource(PGLx, LUTx, FL_PGL_INPUT2, FL_PGL_LUT_INPUT_GROUP0);
        FL_PGL_EnableLUTInputMask(PGLx, LUTx, FL_PGL_INPUT2); //????????????2??????
    }
    else
    {
        FL_PGL_SetLUTInputSource(PGLx, LUTx, FL_PGL_INPUT2, lutInit->input2);
    }
    /*??????1??????*/
    if(lutInit->input1 == FL_PGL_LUT_INPUT_DISABLE)
    {
        FL_PGL_SetLUTInputSource(PGLx, LUTx, FL_PGL_INPUT1, FL_PGL_LUT_INPUT_GROUP0);
        FL_PGL_EnableLUTInputMask(PGLx, LUTx, FL_PGL_INPUT1); //????????????1??????
    }
    else
    {
        FL_PGL_SetLUTInputSource(PGLx, LUTx, FL_PGL_INPUT1, lutInit->input1);
    }
    /*??????0??????*/
    if(lutInit->input0 == FL_PGL_LUT_INPUT_DISABLE)
    {
        FL_PGL_SetLUTInputSource(PGLx, LUTx, FL_PGL_INPUT0, FL_PGL_LUT_INPUT_GROUP0);
        FL_PGL_EnableLUTInputMask(PGLx, LUTx, FL_PGL_INPUT0); //????????????0??????
    }
    else
    {
        FL_PGL_SetLUTInputSource(PGLx, LUTx, FL_PGL_INPUT0, lutInit->input0);
    }
    return FL_PASS;
}

/**
* @brief   ?????? PGL_InitStruct ???????????????
* @param   PGL_InitStruct ??????????????????????????????????????????????????? @ref FL_PGL_LUT_InitTypeDef ?????????
*
* @retval  None
*/
void FL_PGL_StructInit(FL_PGL_LUT_InitTypeDef *PGL_InitStruct)
{
    PGL_InitStruct->edge                = FL_PGL_LUT_EDGE_NONE;
    PGL_InitStruct->outMode             = FL_PGL_LUT_OUTPUT_COMBINE_LOGIC;
    PGL_InitStruct->input3              = FL_PGL_LUT_INPUT_DISABLE;
    PGL_InitStruct->input2              = FL_PGL_LUT_INPUT_DISABLE;
    PGL_InitStruct->input1              = FL_PGL_LUT_INPUT_DISABLE;
    PGL_InitStruct->input0              = FL_PGL_LUT_INPUT_DISABLE;
}


