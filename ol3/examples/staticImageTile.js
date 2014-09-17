// Maps always need a projection, but the static image is not geo-referenced,
// and are only measured in pixels.  So, we create a fake projection that the
// map can use to properly display the layer.
var pixelProjection = new ol.proj.Projection({
  code: 'pixel',
  units: 'pixels',
  extent: [0, 0, 7360,4912]
});

var map = new ol.Map({
  layers: [
    new ol.layer.Image({
      source: new ol.source.ImageTilesStatic({
        attributions: [
          new ol.Attribution({
            html: '&copy; <a href="http://www.s-innovations.net/">xkcd</a>'
          })
        ],
        url: 'https://raw.githubusercontent.com/s-innovations/static_image_tiler/master/data/out/{z}/{x}/{y}.jpg',
        imageSize: [7360,4912],
        projection: pixelProjection,
        imageExtent: pixelProjection.getExtent()
      })
    })
  ],
  target: 'map',
  view: new ol.View({
    projection: pixelProjection,
    center: ol.extent.getCenter(pixelProjection.getExtent()),
    zoom: 2
  })
});
