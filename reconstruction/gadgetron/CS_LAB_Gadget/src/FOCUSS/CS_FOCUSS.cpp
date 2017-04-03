/*
file name	: 	CS_FOCUSS.cpp

author		: 	Martin Schwartz	(martin.schwartz@med.uni-tuebingen.de)

version		: 	1.1

date		: 	13.02.2015

description	: 	implementation of the class "CS_FOCUSS.h"

references	:	-

notes		:	methods, which are included in this file, are same for each inherited sub-class
*/

#include "CS_FOCUSS.h"
#include "GlobalVar_FOCUSS.h"
#include "GadgetIsmrmrdReadWrite.h"

namespace Gadgetron{

// read the XML configuration parameters
int CS_FOCUSS::process_config(ACE_Message_Block* mb){
	#if __GADGETRON_VERSION_HIGHER_3_6__ == 1
		GDEBUG("process config..\n");
	#else
		GADGET_DEBUG1("process config..\n");
	#endif	
	//bXMLControl_ = true;
	#if __GADGETRON_VERSION_HIGHER_3_6__ == 1
		bXMLControl_ = bXMLControl.value();		
	#else
		bXMLControl_ = this->get_int_value("bXMLControl");		
	#endif	
	
	if (bXMLControl_) {

		#if __GADGETRON_VERSION_HIGHER_3_6__ == 1
			GDEBUG("XML Control enabled..\n");		
			iNOuter_ = OuterIterations.value();
		  	iNInner_ = InnerIterations.value();
			bESPRActiveCS_ = CSESPReSSo.value();
			cfLambda_ = lambda.value();			
			cfLambdaESPReSSo_ = lambdaESPReSSo.value();
			int iDimFFT = fftSparseDim.value();
			int iDimDCTSparse = dctSparseDim.value();
			int iDimPCASparse = pcaSparseDim.value();
			int iDimKernelFFT = kernelFftDim.value();
			int iTransformFFTBA = transformFftBaDim.value();
			int ikSpaceOut = kSpaceOutDim.value();
		#else
			GADGET_DEBUG1("XML Control enabled..\n");
		  	iNOuter_ = this->get_int_value("OuterIterations");		  	
		  	iNInner_ = this->get_int_value("InnerIterations");		  
			bESPRActiveCS_ = this->get_int_value("CSESPReSSo");
			cfLambda_ = this->get_double_value("lambda");
			cfLambdaESPReSSo_ = this->get_double_value("lambdaESPReSSo");
			int iDimFFT = this->get_int_value("fftSparseDim");
			int iDimDCTSparse = this->get_int_value("dctSparseDim");
			int iDimPCASparse = this->get_int_value("pcaSparseDim");
			int iDimKernelFFT = this->get_int_value("kernelFftDim");
			int iTransformFFTBA = this->get_int_value("transformFftBaDim");
			int ikSpaceOut = this->get_int_value("kSpaceOutDim");
		#endif

		// update global parameters
		GlobalVar_FOCUSS::instance()->iNOuter_ = iNOuter_;
		GlobalVar_FOCUSS::instance()->iNInner_ = iNInner_;
		GlobalVar_FOCUSS::instance()->bESPRActiveCS_ = bESPRActiveCS_;
		GlobalVar_FOCUSS::instance()->cfLambda_ = cfLambda_;	
		GlobalVar_FOCUSS::instance()->cfLambdaESPReSSo_ = cfLambdaESPReSSo_;
		GlobalVar_FOCUSS::instance()->iDimFFT_ = iDimFFT;
		GlobalVar_FOCUSS::instance()->iDimDCTSparse_ = iDimDCTSparse;
		GlobalVar_FOCUSS::instance()->iDimPCASparse_ = iDimPCASparse;
		GlobalVar_FOCUSS::instance()->iDimKernelFFT_ = iDimKernelFFT;
		GlobalVar_FOCUSS::instance()->iTransformFFTBA_ = iTransformFFTBA;
		GlobalVar_FOCUSS::instance()->ikSpaceOut_ = ikSpaceOut;
	}
	else{
		iNOuter_ = GlobalVar_FOCUSS::instance()->iNOuter_;
		iNInner_ = GlobalVar_FOCUSS::instance()->iNInner_;
		bESPRActiveCS_ = GlobalVar_FOCUSS::instance()->bESPRActiveCS_;
		iVDMap_ = GlobalVar_FOCUSS::instance()->iVDMap_;
		fFullySampled_ = GlobalVar_FOCUSS::instance()->fFullySampled_;
		cfLambdaESPReSSo_ = GlobalVar_FOCUSS::instance()->cfLambdaESPReSSo_;
		cfLambda_ = GlobalVar_FOCUSS::instance()->cfLambda_;
		iESPReSSoDirection_ = GlobalVar_FOCUSS::instance()->iESPReSSoDirection_;
		fPartialFourierVal_ = GlobalVar_FOCUSS::instance()->fPartialFourierVal_;
	
	}
	
	#if __GADGETRON_VERSION_HIGHER_3_6__ == 1
		GDEBUG("lambda is %f \n", GlobalVar_FOCUSS::instance()->cfLambda_.real());
		GDEBUG("Lambda ESPReSSo is %f \n", GlobalVar_FOCUSS::instance()->cfLambdaESPReSSo_.real());
		GDEBUG("Fully Sampled is %f \n", GlobalVar_FOCUSS::instance()->fFullySampled_);
		GDEBUG("bESPRActiveCS is %i \n", GlobalVar_FOCUSS::instance()->bESPRActiveCS_);
		GDEBUG("kSpaceOutDim is %i \n", GlobalVar_FOCUSS::instance()->ikSpaceOut_);
		GDEBUG("transformFftBaDim is %i \n", GlobalVar_FOCUSS::instance()->iTransformFFTBA_);
		GDEBUG("kernelFftDim is %i \n", GlobalVar_FOCUSS::instance()->iDimKernelFFT_);
		GDEBUG("pcaSparseDim is %i \n", GlobalVar_FOCUSS::instance()->iDimPCASparse_);
		GDEBUG("dctSparseDim is %i \n", GlobalVar_FOCUSS::instance()->iDimDCTSparse_);
		GDEBUG("fftSparseDim is %i  \n", GlobalVar_FOCUSS::instance()->iDimFFT_);
		GDEBUG("InnerIterations is %i \n", GlobalVar_FOCUSS::instance()->iNInner_);
		GDEBUG("OuterIterations is %i \n", GlobalVar_FOCUSS::instance()->iNOuter_);
	#else
		GADGET_DEBUG2("lambda is %f \n", GlobalVar_FOCUSS::instance()->cfLambda_);
		GADGET_DEBUG2("Lambda ESPReSSo is %f \n", GlobalVar_FOCUSS::instance()->cfLambdaESPReSSo_);
		GADGET_DEBUG2("Fully Sampled is %f \n", GlobalVar_FOCUSS::instance()->fFullySampled_);
		GADGET_DEBUG2("bESPRActiveCS is %i \n", GlobalVar_FOCUSS::instance()->bESPRActiveCS_);
		GADGET_DEBUG2("kSpaceOutDim is %i \n", GlobalVar_FOCUSS::instance()->ikSpaceOut_);
		GADGET_DEBUG2("transformFftBaDim is %i \n", GlobalVar_FOCUSS::instance()->iTransformFFTBA_);
		GADGET_DEBUG2("kernelFftDim is %i \n", GlobalVar_FOCUSS::instance()->iDimKernelFFT_);
		GADGET_DEBUG2("pcaSparseDim is %i \n", GlobalVar_FOCUSS::instance()->iDimPCASparse_);
		GADGET_DEBUG2("dctSparseDim is %i \n", GlobalVar_FOCUSS::instance()->iDimDCTSparse_);
		GADGET_DEBUG2("fftSparseDim is %i  \n", GlobalVar_FOCUSS::instance()->iDimFFT_);
		GADGET_DEBUG2("InnerIterations is %i \n", GlobalVar_FOCUSS::instance()->iNInner_);
		GADGET_DEBUG2("OuterIterations is %i \n", GlobalVar_FOCUSS::instance()->iNOuter_);
	#endif

	if (iNInner_ <= 0) iNInner_ = 20;
	if (iNOuter_ <= 0) iNOuter_ = 2;	

	// p-value for the lp-norm
	fP_ = .5;

	// convergence boundary
	fEpsilon_ = (float)1e-6;

	// setup of the transformation parameters - sparsity dim, fft dim, ..
	fSetupTransformation();

	return GADGET_OK;
};


// set several variables
void CS_FOCUSS::fInitVal(GadgetContainerMessage< ISMRMRD::ImageHeader >* m1){

	// initialize the global vector variables
	/*for(int i = 0; i < 20; i++){
		GlobalVar_FOCUSS::instance()->vbStatPrinc_.push_back(false);
		GlobalVar_FOCUSS::instance()->vfPrincipleComponents_.push_back(new hoNDArray<std::complex< float > > ());
	}*/

}

void CS_FOCUSS::fSetupTransformation(){

	// instantiate transformation objects
	Transform_KernelTransform_	= new Transform();
	Transform_fftBA_		= new Transform();
	Transform_fftAA_		= new Transform();

	int dim;
	/*-------------------------------------------------------------------------
	---------------------------- KernelTransform ------------------------------
	--------------------------------------------------------------------------*/
	// KernelTransform - active
	Transform_KernelTransform_->set_active();

	// configure KernelTransformation - sparsifying transform
	// check FFT entry
	dim = GlobalVar_FOCUSS::instance()->iDimFFT_;
	if (dim != 0){
		for(int i = 0; i < 7; i++){
			int bit = (dim & (1 << i)) >> i;
			if (bit == 1) {
				Transform_KernelTransform_->set_transformation_sparsity(0,i);

				#if __GADGETRON_VERSION_HIGHER_3_6__ == 1
					GDEBUG("KernelTransform - FFT sparse - dim: %i \n", i);
				#else
					GADGET_DEBUG2("KernelTransform - FFT sparse - dim: %i \n", i);
				#endif
			}
		}
	}

	// check DCT entry
	dim = GlobalVar_FOCUSS::instance()->iDimDCTSparse_;
	if (dim != 0){
		for(int i = 0; i < 7; i++){
			int bit = (dim & (1 << i)) >> i;
			if (bit == 1) {
				Transform_KernelTransform_->set_transformation_sparsity(1,i);
				#if __GADGETRON_VERSION_HIGHER_3_6__ == 1
					GDEBUG("KernelTransform - DCT sparse - dim: %i \n", i);
				#else
					GADGET_DEBUG2("KernelTransform - DCT sparse - dim: %i \n", i);
				#endif
			}
		}
	}

	// check PCA entry
	/*if 
	dim = GlobalVar_FOCUSS::instance()->iDimPCASparse_;
	if (dim != 0)
	{
		for(int i = 0; i < 7; i++){
			int bit = (dim & (1 << i)) >> i;
			if (bit == 1){
				Transform_KernelTransform_->set_transformation_sparsity(2,i);
				GADGET_DEBUG2("KernelTransform - PCA sparse - dim: %i \n", i);
			}
		}
	}
	*/

	// configure KernelTransformation - FFT
	dim = GlobalVar_FOCUSS::instance()->iDimKernelFFT_;
	if (dim != 0){
		for(int i = 0; i < 7; i++){
			int bit = (dim & (1 << i)) >> i;
			if (bit == 1){
				Transform_KernelTransform_->set_transformation_fft(i);
				#if __GADGETRON_VERSION_HIGHER_3_6__ == 1
					GDEBUG("KernelTransform - FFT - dim: %i \n", i);
				#else
					GADGET_DEBUG2("KernelTransform - FFT - dim: %i \n", i);
				#endif
			}
		}
	}

	/*-------------------------------------------------------------------------
	---------------------------------- fftBA ----------------------------------
	--------------------------------------------------------------------------*/
	// configure fftBA - transform dimension before start FOCUSS
	dim = GlobalVar_FOCUSS::instance()->iTransformFFTBA_;
	if (dim != 0){
		for(int i = 0; i < 7; i++){
			int bit = (dim & (1 << i)) >> i;
			if (bit == 1){
				Transform_fftBA_->set_transformation_fft(i);
				#if __GADGETRON_VERSION_HIGHER_3_6__ == 1
					GDEBUG("fftBA - dim: %i \n", i);
				#else
					GADGET_DEBUG2("fftBA - dim: %i \n", i);
				#endif
			}
		}
		Transform_fftBA_->set_active();
	}

	// configure fftAA - output image or k-space
	dim = GlobalVar_FOCUSS::instance()->ikSpaceOut_;
	if (dim != 0){
		Transform_fftAA_->set_transformation_sparsity(0,0);
		Transform_fftAA_->set_transformation_sparsity(0,1);
		Transform_fftAA_->set_transformation_sparsity(0,2);
		Transform_fftAA_->set_transformation_fft(0);
		Transform_fftAA_->set_transformation_fft(1);
		Transform_fftAA_->set_transformation_fft(2);
		Transform_fftAA_->set_active();
		#if __GADGETRON_VERSION_HIGHER_3_6__ == 1
			GDEBUG("Transform_fftAA_ - active: %i \n", Transform_fftAA_->get_active());
		#else
			GADGET_DEBUG2("Transform_fftAA_ - active: %i \n", Transform_fftAA_->get_active());
		#endif
	}
}

}