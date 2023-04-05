#include "ismrmrd/ismrmrd.h"
#include <boost/test/unit_test.hpp>
#include <any>

template<typename T>
class MockImage : public ISMRMRD::Image<T> {
public:
   using ISMRMRD::Image<T>::Image;

   ISMRMRD::ISMRMRD_Image *getPim() { return this->pim.get(); }

   ISMRMRD::ISMRMRD_Image &getIm() { return this->im; }
};

BOOST_AUTO_TEST_SUITE(IsmrmrdTest, *boost::unit_test::enabled())

   BOOST_AUTO_TEST_CASE(test_image_move_constructor) {
      MockImage<short> img1(1, 1, 1);
      ISMRMRD::ISMRMRD_Image *pim1 = img1.getPim();
      ISMRMRD::ISMRMRD_Image &im1 = img1.getIm();
      BOOST_CHECK_EQUAL(&im1, pim1);
      MockImage<short> img2(std::move(img1));
      BOOST_CHECK(!img1.getPim());
      ISMRMRD::ISMRMRD_Image *pim2 = img2.getPim();
      BOOST_CHECK_EQUAL(pim1, pim2);
      ISMRMRD::ISMRMRD_Image &im2 = img2.getIm();
      BOOST_CHECK_EQUAL(&im2, pim2);
   }

   BOOST_AUTO_TEST_CASE(test_image_destructor) {
      auto pimg = new MockImage<short>(1);
      MockImage<short> newImg(std::move(*pimg));
      delete pimg;
      BOOST_CHECK(newImg.getDataPtr());
   }

   BOOST_AUTO_TEST_CASE(test_image_move_assignment, *boost::unit_test::disabled()) {
      MockImage<short> img1(1, 1, 1);
      ISMRMRD::ISMRMRD_Image *pim1 = img1.getPim();
      MockImage<short> img2;
      img2 = std::move(img1);
      BOOST_CHECK(!img1.getPim());
      ISMRMRD::ISMRMRD_Image *pim2 = img2.getPim();
      BOOST_CHECK_EQUAL(pim1, pim2);
   }

   BOOST_AUTO_TEST_CASE(test_struct_constructor) {
      auto pim = std::make_unique<ISMRMRD::ISMRMRD_Image>();
      auto orig_ptr = pim.get();
      ismrmrd_init_image(pim.get());
      pim->head.data_type = ISMRMRD::ISMRMRD_SHORT;
      MockImage<short> img1(std::move(pim));
      BOOST_CHECK_EQUAL(img1.getPim(), orig_ptr);
   }

BOOST_AUTO_TEST_SUITE_END()