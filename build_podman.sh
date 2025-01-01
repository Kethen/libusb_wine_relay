set -xe
IMAGE_TAG="libusb_wine_relay_build"

if [ -n "$REBUILD_IMAGE" ]
then
	podman image rm -f $IMAGE_TAG
fi

if ! podman image exists $IMAGE_TAG
then
	podman image build -t $IMAGE_TAG -f Dockerfile
fi

podman run \
	--rm -it \
	--security-opt label=disable \
	-v ./:/workdir \
	-v ./build.sh:/workdir/build.sh:ro \
	-w /workdir \
	--entrypoint /bin/bash \
	$IMAGE_TAG \
	build.sh
