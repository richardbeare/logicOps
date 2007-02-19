#ifndef __itkBinaryOrImageFilter_h
#define __itkBinaryOrImageFilter_h

#include "itkBinaryFunctorImageFilter.h"
#include "itkNumericTraits.h"


namespace itk
{
  
/** \class BinaryOrImageFilter
 * \brief Implements the BinaryOr logical operator pixel-wise between two images.
 *
 * This class is parametrized over the types of the two 
 * input images and the type of the output image. 
 * Numeric conversions (castings) are done by the C++ defaults.
 *
 * 
 * The total operation over one pixel will be
 *
 *  output_pixel = static_cast<PixelType>( input1_pixel != input2_pixel )
 *
 * Where "!=" is the equality operator in C++.
 *
 * \ingroup IntensityImageFilters  Multithreaded
 */
namespace Functor {  
  
template< class TPixel >
class BinaryOr
{
public:
  BinaryOr() {};
  ~BinaryOr() {};
  bool operator!=( const BinaryOr & ) const
    {
    return false;
    }
  bool operator==( const BinaryOr & other ) const
    {
    return !(*this != other);
    }
  inline TPixel operator()( const TPixel & A, const TPixel & B)
    {
    bool a = ( A == m_ForegroundValue );
    bool b = ( B == m_ForegroundValue );
    if( a || b )
      {
      return m_ForegroundValue;
      }
    return m_BackgroundValue;
    }

  TPixel m_ForegroundValue;
  TPixel m_BackgroundValue;
}; 

}
template <class TImage>
class ITK_EXPORT BinaryOrImageFilter :
    public
BinaryFunctorImageFilter<TImage, TImage, TImage, 
  Functor::BinaryOr< typename TImage::PixelType > >


{
public:
  /** Standard class typedefs. */
  typedef BinaryOrImageFilter  Self;
  typedef BinaryFunctorImageFilter<TImage, TImage,TImage, 
    Functor::BinaryOr<  typename TImage::PixelType> > Superclass;
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef typename TImage::PixelType     PixelType;

  /** Set the value in the image to consider as "foreground". Defaults to
   * maximum value of PixelType.*/
  itkSetMacro(ForegroundValue, PixelType);

  /** Get the value in the image considered as "foreground". Defaults to
   * maximum value of PixelType. */
  itkGetConstMacro(ForegroundValue, PixelType);

  /** Set the value used as "background". Defaults to
   * NumericTraits<PixelType>::NonpositiveMin(). */
  itkSetMacro(BackgroundValue, PixelType);

  /** Get the value used as "background". Defaults to
   * NumericTraits<PixelType>::NonpositiveMin(). */
  itkGetConstMacro(BackgroundValue, PixelType);
  

protected:
  BinaryOrImageFilter()
    {
    m_ForegroundValue = NumericTraits<PixelType>::max();
    m_BackgroundValue = NumericTraits<PixelType>::NonpositiveMin();
    }
  virtual ~BinaryOrImageFilter() {}

  void PrintSelf(std::ostream& os, Indent indent) const
    {
    Superclass::PrintSelf(os,indent);

    typedef typename NumericTraits<PixelType>::PrintType
                                              PixelPrintType;

    os << indent << "ForegroundValue: " 
                    << static_cast< PixelPrintType > (m_ForegroundValue) 
                    << std::endl;
 
    os << indent << "BackgroundValue: " 
                    << static_cast< PixelPrintType > (m_BackgroundValue) 
                    << std::endl;
    }

  void GenerateData()
    {
    this->GetFunctor().m_ForegroundValue = m_ForegroundValue;
    this->GetFunctor().m_BackgroundValue = m_BackgroundValue;
    Superclass::GenerateData();
    }

private:
  BinaryOrImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  PixelType m_ForegroundValue;
  PixelType m_BackgroundValue;
  
};

} // end namespace itk


#endif
