
/* Copyright (c) 2014-2015, Human Brain Project
 *                          Daniel Nachbaur <daniel.nachbaur@epfl.ch>
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

#define BOOST_TEST_MODULE hbp_serialization

#include <zeroeq/hbp/vocabulary.h>
#include <zeroeq/zeroeq.h>

#include <boost/test/unit_test.hpp>

typedef std::vector< uint32_t > uint32_ts;

BOOST_AUTO_TEST_CASE( cameraEvent )
{
    const std::vector< float > camera( 16, 42 );
    const zeroeq::Event& event = zeroeq::hbp::serializeCamera( camera );
    const std::vector< float >& deserialized =
            zeroeq::hbp::deserializeCamera( event );
    BOOST_CHECK_EQUAL_COLLECTIONS( camera.begin(), camera.end(),
                                   deserialized.begin(), deserialized.end( ));
}

BOOST_AUTO_TEST_CASE( frameEvent )
{
    {
        const zeroeq::hbp::data::Frame frame( 1, 2, 3, 0 );
        const zeroeq::Event& event = zeroeq::hbp::serializeFrame( frame );
        const auto& out = zeroeq::hbp::deserializeFrame( event );

        BOOST_CHECK_EQUAL( frame, out );

        std::cout << frame << std::endl; // increase coverage
    }
    {
        const zeroeq::hbp::data::Frame frame( 3, 2, 1, -1 );
        const zeroeq::Event& event = zeroeq::hbp::serializeFrame( frame );
        const auto& out = zeroeq::hbp::deserializeFrame( event );
        const zeroeq::hbp::data::Frame expected( 3, 3, 3, -1 );

        BOOST_CHECK_EQUAL( expected, out );
    }
}

BOOST_AUTO_TEST_CASE( selectionsEvent )
{
    unsigned int ids[] = {16,2,77,29};
    const std::vector< unsigned int > selection(
        ids, ids + sizeof(ids) / sizeof(unsigned int) );
    const zeroeq::Event& selectionEvent =
        zeroeq::hbp::serializeSelectedIDs( selection );
    const std::vector< unsigned int >& deserializedSelection =
            zeroeq::hbp::deserializeSelectedIDs( selectionEvent );
    BOOST_CHECK_EQUAL_COLLECTIONS(
        selection.begin(), selection.end(),
        deserializedSelection.begin(), deserializedSelection.end( ));
}

BOOST_AUTO_TEST_CASE( toggleRequestEvent )
{
    unsigned int ids[] = {16,2,77,29};
    const std::vector< unsigned int > selection(
        ids, ids + sizeof(ids) / sizeof(unsigned int) );
    const zeroeq::Event& toggleRequest_event =
        zeroeq::hbp::serializeToggleIDRequest( selection );
    const std::vector< unsigned int >& deserialized_toggleRequest =
            zeroeq::hbp::deserializeToggleIDRequest( toggleRequest_event );
    BOOST_CHECK_EQUAL_COLLECTIONS(
        selection.begin(), selection.end(),
        deserialized_toggleRequest.begin(), deserialized_toggleRequest.end( ));
}

BOOST_AUTO_TEST_CASE( lookupTable1D )
{
    const std::vector< uint8_t > lut( 1024 );
    const zeroeq::Event& lookupTableEvent =
            zeroeq::hbp::serializeLookupTable1D( lut );
    const std::vector< uint8_t >& deserializedLut =
            zeroeq::hbp::deserializeLookupTable1D( lookupTableEvent );
    BOOST_CHECK_EQUAL_COLLECTIONS(
        lut.begin(), lut.end(), deserializedLut.begin(), deserializedLut.end());
}

BOOST_AUTO_TEST_CASE( imageJPEGEvent )
{
    const size_t size = 24;
    const uint8_t imageJPEGData[ size ] = { 13, 11, 17, 19, 34, 73, 25, 24, 36,
                                            74, 21, 56, 78, 23, 42, 23, 24, 42,
                                            74, 32, 12, 35, 35, 13 };
    zeroeq::hbp::data::ImageJPEG image( size, &imageJPEGData[0] );

    const zeroeq::Event& imageEvent = zeroeq::hbp::serializeImageJPEG( image );
    const zeroeq::hbp::data::ImageJPEG& deserializedImage =
            zeroeq::hbp::deserializeImageJPEG( imageEvent );
    BOOST_CHECK_EQUAL( image.getSizeInBytes(),
                       deserializedImage.getSizeInBytes( ));
    BOOST_CHECK_EQUAL_COLLECTIONS( imageJPEGData, imageJPEGData + size,
                                   deserializedImage.getDataPtr(),
                                   deserializedImage.getDataPtr() + size );
}

BOOST_AUTO_TEST_CASE( cellSetBinaryOp )
{
  zeroeq::hbp::data::CellSetBinaryOp cellSet (
      { 0, 2, 4, 6 }, { 1, 3, 5, 7 },
      zeroeq::hbp::CELLSETOP_SYNAPTIC_PROJECTIONS );

  const zeroeq::Event& cellSetBinaryOpEvent =
      zeroeq::hbp::serializeCellSetBinaryOp( cellSet.first, cellSet.second,
                                          cellSet.operation );

  zeroeq::hbp::data::CellSetBinaryOp deserializedCellSetBinaryOp =
      zeroeq::hbp::deserializeCellSetBinaryOp( cellSetBinaryOpEvent );

  BOOST_CHECK_EQUAL( cellSet.operation,
                     deserializedCellSetBinaryOp.operation);

  BOOST_CHECK_EQUAL_COLLECTIONS( cellSet.first.begin( ),
                                 cellSet.first.end( ),
                                 deserializedCellSetBinaryOp.first.begin( ),
                                 deserializedCellSetBinaryOp.first.end( ));

  BOOST_CHECK_EQUAL_COLLECTIONS( cellSet.second.begin( ),
                                 cellSet.second.end( ),
                                 deserializedCellSetBinaryOp.second.begin( ),
                                 deserializedCellSetBinaryOp.second.end( ));
}
