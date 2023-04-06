#include "ismrmrd/ismrmrd.h"
#include <boost/test/unit_test.hpp>

template<typename T>
class MockImage : public ISMRMRD::Image<T> {
public:
   using ISMRMRD::Image<T>::Image;
   ISMRMRD::ISMRMRD_Image &getIm() { return this->im; }
};

BOOST_AUTO_TEST_SUITE(IsmrmrdTest, *boost::unit_test::enabled())

   BOOST_AUTO_TEST_CASE(test_image_move_constructor) {
      MockImage<short> img1(1);
      img1.getHead().measurement_uid = 1;
      img1.setAttributeString("test");
      ISMRMRD::ISMRMRD_Image im1 = img1.getIm();
      MockImage<short> img2(std::move(img1));
      BOOST_CHECK(!img1.getIm().data); // NOLINT(bugprone-use-after-move)
      BOOST_CHECK(!img1.getIm().attribute_string);
      BOOST_CHECK_EQUAL(img1.getIm().head.attribute_string_len, 0);
      BOOST_CHECK_EQUAL(img2.getIm().data, im1.data);
      BOOST_CHECK_EQUAL(img2.getIm().attribute_string, im1.attribute_string);
      BOOST_CHECK_EQUAL(img2.getIm().head.measurement_uid, im1.head.measurement_uid);
   }

   BOOST_AUTO_TEST_CASE(test_image_move_assignment) {
      MockImage<short> img1(1);
      img1.getHead().measurement_uid = 1;
      img1.setAttributeString("test");
      ISMRMRD::ISMRMRD_Image im1_orig = img1.getIm();
      MockImage<short> img2(1);
      img2 = std::move(img1);
      BOOST_CHECK(!img1.getIm().data); // NOLINT(bugprone-use-after-move)
      BOOST_CHECK(!img1.getIm().attribute_string);
      BOOST_CHECK_EQUAL(img1.getIm().head.attribute_string_len, 0);
      BOOST_CHECK_EQUAL(img2.getIm().data, im1_orig.data);
      BOOST_CHECK_EQUAL(img2.getIm().attribute_string, im1_orig.attribute_string);
      BOOST_CHECK_EQUAL(img2.getIm().head.measurement_uid, im1_orig.head.measurement_uid);
   }

   BOOST_AUTO_TEST_CASE(test_struct_constructor) {
      auto pim = std::unique_ptr<ISMRMRD::ISMRMRD_Image>(new ISMRMRD::ISMRMRD_Image);
      ismrmrd_init_image(pim.get());
      pim->head.matrix_size[0] = 1;
      pim->head.data_type = ISMRMRD::ISMRMRD_SHORT;
      pim->head.attribute_string_len = 4;
      pim->head.measurement_uid = 1;
      ismrmrd_make_consistent_image(pim.get());
      ISMRMRD::ISMRMRD_Image im_orig = *pim;
      MockImage<short> img(std::move(pim));
      BOOST_CHECK_EQUAL(img.getIm().data, im_orig.data);
      BOOST_CHECK_EQUAL(img.getIm().attribute_string, im_orig.attribute_string);
      BOOST_CHECK_EQUAL(img.getIm().head.measurement_uid, im_orig.head.measurement_uid);
   }

BOOST_AUTO_TEST_SUITE_END()