// swift-tools-version:5.3

import PackageDescription
import Foundation

let package = Package(
    name: "GMI",
    platforms: [
        .iOS(.v13)
    ],
    products: [
        .library(
            name: "GMIPackage",
            targets: ["GMIPackage"])
        
    ],
    dependencies: [
        .package(name: "Realm", url: "https://github.com/realm/realm-cocoa", .upToNextMajor(from:Version("10.10.0")))
    ],
    targets: [
        .target(name: "GMIPackage",
                        dependencies: [
                            .target(name: "GMI", condition: .when(platforms: .some([.iOS]))),
                            .target(name: "IWS_Shared_Components", condition: .when(platforms: .some([.iOS]))),
                            .target(name: "PalmAPI", condition: .when(platforms: .some([.iOS]))),
                            .product(name: "RealmSwift", package: "Realm"),
                            .product(name: "Realm", package: "Realm")
                        ],
                        path: "GMIPath"
                ),
        .binaryTarget(name: "GMI", url: "https://github.com/ImageWare/ios_iwa_sdk/raw/cb/update-to-1.6/GMI.xcframework.zip", checksum: "08b3cd5c107fb3349b73fde14484345b97a008919e5bca092775330ae8b524c2"),
        .binaryTarget(name: "IWS_Shared_Components",
                      path: "IWS_Shared_Components.xcframework"),
        
        .binaryTarget(name: "PalmAPI", path: "PalmAPI.xcframework")
    ],
    swiftLanguageVersions: [.v5]
)
